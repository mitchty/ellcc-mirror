//===--- InitPreprocessor.cpp - PP initialization code. ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the ellcc::InitializePreprocessor function.
//
//===----------------------------------------------------------------------===//

#include "InitPreprocessor.h"
#include "TargetInfo.h"
#include "Preprocessor.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/System/Path.h"

namespace ellcc {

// Append a #define line to Buf for Macro.  Macro should be of the form XXX,
// in which case we emit "#define XXX 1" or "XXX=Y z W" in which case we emit
// "#define XXX Y z W".  To get a #define with no value, use "XXX=".
static void DefineBuiltinMacro(std::vector<char> &Buf, const char *Macro,
                               const char *Command = "#define ") {
  Buf.insert(Buf.end(), Command, Command+strlen(Command));
  if (const char *Equal = strchr(Macro, '=')) {
    // Turn the = into ' '.
    Buf.insert(Buf.end(), Macro, Equal);
    Buf.push_back(' ');
    
    // Per GCC -D semantics, the macro ends at \n if it exists.
    const char *End = strpbrk(Equal, "\n\r");
    if (End) {
      fprintf(stderr, "warning: macro '%s' contains embedded newline, text "
              "after the newline is ignored.\n",
              std::string(Macro, Equal).c_str());
    } else {
      End = Equal+strlen(Equal);
    }
    
    Buf.insert(Buf.end(), Equal+1, End);
  } else {
    // Push "macroname 1".
    Buf.insert(Buf.end(), Macro, Macro+strlen(Macro));
    Buf.push_back(' ');
    Buf.push_back('1');
  }
  Buf.push_back('\n');
}

// Append a #undef line to Buf for Macro.  Macro should be of the form XXX
// and we emit "#undef XXX".
static void UndefineBuiltinMacro(std::vector<char> &Buf, const char *Macro) {
  // Push "macroname".
  const char *Command = "#undef ";
  Buf.insert(Buf.end(), Command, Command+strlen(Command));
  Buf.insert(Buf.end(), Macro, Macro+strlen(Macro));
  Buf.push_back('\n');
}

/// Add the quoted name of an implicit include file.
static void AddQuotedIncludePath(std::vector<char> &Buf, 
                                 const std::string &File) {
  // Implicit include paths should be resolved relative to the current
  // working directory first, and then use the regular header search
  // mechanism. The proper way to handle this is to have the
  // predefines buffer located at the current working directory, but
  // it has not file entry. For now, workaround this by using an
  // absolute path if we find the file here, and otherwise letting
  // header search handle it.
  llvm::sys::Path Path(File);
  Path.makeAbsolute();
  if (!Path.exists())
    Path = File;
    
  // Escape double quotes etc.
  Buf.push_back('"');
  std::string EscapedFile = Lexer::Stringify(Path.toString());
  Buf.insert(Buf.end(), EscapedFile.begin(), EscapedFile.end());
  Buf.push_back('"');
}

/// AddImplicitInclude - Add an implicit #include of the specified file to the
/// predefines buffer.
static void AddImplicitInclude(std::vector<char> &Buf, 
                               const std::string &File) {
  const char *Inc = "#include ";
  Buf.insert(Buf.end(), Inc, Inc+strlen(Inc));
  AddQuotedIncludePath(Buf, File);
  Buf.push_back('\n');
}

static void AddImplicitIncludeMacros(std::vector<char> &Buf,
                                     const std::string &File) {
  const char *Inc = "#__include_macros ";
  Buf.insert(Buf.end(), Inc, Inc+strlen(Inc));
  AddQuotedIncludePath(Buf, File);
  Buf.push_back('\n');
  // Marker token to stop the __include_macros fetch loop.
  const char *Marker = "##\n"; // ##?
  Buf.insert(Buf.end(), Marker, Marker+strlen(Marker));
}

/// AddImplicitIncludePTH - Add an implicit #include using the original file
///  used to generate a PTH cache.
static void AddImplicitIncludePTH(std::vector<char> &Buf, Preprocessor &PP, 
  const std::string& ImplicitIncludePTH) {
  PTHManager *P = PP.getPTHManager();
  assert(P && "No PTHManager.");
  const char *OriginalFile = P->getOriginalSourceFile();
  
  if (!OriginalFile) {
    assert(!ImplicitIncludePTH.empty());
    fprintf(stderr, "error: PTH file '%s' does not designate an original "
            "source header file for -include-pth\n",
            ImplicitIncludePTH.c_str());
    exit (1);
  }
  
  AddImplicitInclude(Buf, OriginalFile);
}

/// PickFP - This is used to pick a value based on the FP semantics of the
/// specified FP model.
template <typename T>
static T PickFP(const llvm::fltSemantics *Sem, T IEEESingleVal,
                T IEEEDoubleVal, T X87DoubleExtendedVal, T PPCDoubleDoubleVal) {
  if (Sem == &llvm::APFloat::IEEEsingle)
    return IEEESingleVal;
  if (Sem == &llvm::APFloat::IEEEdouble)
    return IEEEDoubleVal;
  if (Sem == &llvm::APFloat::x87DoubleExtended)
    return X87DoubleExtendedVal;
  assert(Sem == &llvm::APFloat::PPCDoubleDouble);
  return PPCDoubleDoubleVal;
}

static void DefineFloatMacros(std::vector<char> &Buf, const char *Prefix,
                              const llvm::fltSemantics *Sem) {
  const char *DenormMin, *Epsilon, *Max, *Min;
  DenormMin = PickFP(Sem, "1.40129846e-45F", "4.9406564584124654e-324", 
                     "3.64519953188247460253e-4951L",
                     "4.94065645841246544176568792868221e-324L");
  int Digits = PickFP(Sem, 6, 15, 18, 31);
  Epsilon = PickFP(Sem, "1.19209290e-7F", "2.2204460492503131e-16",
                   "1.08420217248550443401e-19L",
                   "4.94065645841246544176568792868221e-324L");
  int HasInifinity = 1, HasQuietNaN = 1;
  int MantissaDigits = PickFP(Sem, 24, 53, 64, 106);
  int Min10Exp = PickFP(Sem, -37, -307, -4931, -291);
  int Max10Exp = PickFP(Sem, 38, 308, 4932, 308);
  int MinExp = PickFP(Sem, -125, -1021, -16381, -968);
  int MaxExp = PickFP(Sem, 128, 1024, 16384, 1024);
  Min = PickFP(Sem, "1.17549435e-38F", "2.2250738585072014e-308",
               "3.36210314311209350626e-4932L",
               "2.00416836000897277799610805135016e-292L");
  Max = PickFP(Sem, "3.40282347e+38F", "1.7976931348623157e+308",
               "1.18973149535723176502e+4932L",
               "1.79769313486231580793728971405301e+308L");
  
  char MacroBuf[100];
  sprintf(MacroBuf, "__%s_DENORM_MIN__=%s", Prefix, DenormMin);
  DefineBuiltinMacro(Buf, MacroBuf);
  sprintf(MacroBuf, "__%s_DIG__=%d", Prefix, Digits);
  DefineBuiltinMacro(Buf, MacroBuf);
  sprintf(MacroBuf, "__%s_EPSILON__=%s", Prefix, Epsilon);
  DefineBuiltinMacro(Buf, MacroBuf);
  sprintf(MacroBuf, "__%s_HAS_INFINITY__=%d", Prefix, HasInifinity);
  DefineBuiltinMacro(Buf, MacroBuf);
  sprintf(MacroBuf, "__%s_HAS_QUIET_NAN__=%d", Prefix, HasQuietNaN);
  DefineBuiltinMacro(Buf, MacroBuf);
  sprintf(MacroBuf, "__%s_MANT_DIG__=%d", Prefix, MantissaDigits);
  DefineBuiltinMacro(Buf, MacroBuf);
  sprintf(MacroBuf, "__%s_MAX_10_EXP__=%d", Prefix, Max10Exp);
  DefineBuiltinMacro(Buf, MacroBuf);
  sprintf(MacroBuf, "__%s_MAX_EXP__=%d", Prefix, MaxExp);
  DefineBuiltinMacro(Buf, MacroBuf);
  sprintf(MacroBuf, "__%s_MAX__=%s", Prefix, Max);
  DefineBuiltinMacro(Buf, MacroBuf);
  sprintf(MacroBuf, "__%s_MIN_10_EXP__=(%d)", Prefix, Min10Exp);
  DefineBuiltinMacro(Buf, MacroBuf);
  sprintf(MacroBuf, "__%s_MIN_EXP__=(%d)", Prefix, MinExp);
  DefineBuiltinMacro(Buf, MacroBuf);
  sprintf(MacroBuf, "__%s_MIN__=%s", Prefix, Min);
  DefineBuiltinMacro(Buf, MacroBuf);
  sprintf(MacroBuf, "__%s_HAS_DENORM__=1", Prefix);
  DefineBuiltinMacro(Buf, MacroBuf);
}


/// DefineTypeSize - Emit a macro to the predefines buffer that declares a macro
/// prefixed MacroName with the maximum and minimum value for a type with width
/// 'TypeWidth' a signedness of 'isSigned' and with a value suffix of 'ValSuffix' (e.g. LL).
static void DefineTypeSize(const char *MacroName, unsigned TypeWidth,
                           const char *ValSuffix, bool isSigned,
                           std::vector<char> &Buf,
                           bool force = false)
{
    char MacroBuf[60];
    long long MaxVal = 0;
    long long MinVal = 0;
    if (isSigned) {
        MaxVal = (1LL << (TypeWidth - 1)) - 1;
        MinVal = -(1LL << (TypeWidth - 1));
    } else {
        MaxVal = ~0ULL >> (64-TypeWidth);
    }
  
    sprintf(MacroBuf, "%s_MAX__=%llu%s", MacroName, MaxVal, ValSuffix);
    DefineBuiltinMacro(Buf, MacroBuf);
    if (isSigned || force) {
        sprintf(MacroBuf, "%s_MIN__=%lld%s", MacroName, MinVal, ValSuffix);
    }
    DefineBuiltinMacro(Buf, MacroBuf);
}

static void DefineType(const char *MacroName, TargetInfo::TypeID Ty, std::vector<char> &Buf) {
  char MacroBuf[60];
  sprintf(MacroBuf, "%s=%s", MacroName, TargetInfo::getTypeName(Ty));
  DefineBuiltinMacro(Buf, MacroBuf);
}


static void InitializePredefinedMacros(const TargetInfo &TI,
                                       const LangOptions &LangOpts,
                                       std::vector<char> &Buf) {
  char MacroBuf[60];
  // Compiler version introspection macros.
  DefineBuiltinMacro(Buf, "__llvm__=1");   // LLVM Backend
  DefineBuiltinMacro(Buf, "__ellcc__=1");  // ELLCC Frontend
  
  // Currently claim to be compatible with GCC 4.3.2
  DefineBuiltinMacro(Buf, "__APPLE_CC__=5621");
  DefineBuiltinMacro(Buf, "__GNUC_MINOR__=3");
  DefineBuiltinMacro(Buf, "__GNUC_PATCHLEVEL__=2");
  DefineBuiltinMacro(Buf, "__GNUC__=4");
  DefineBuiltinMacro(Buf, "__GXX_ABI_VERSION=1002");
  DefineBuiltinMacro(Buf, "__VERSION__=\"4.3.2 Compatible Ecc Compiler\"");
  
  
  // Initialize language-specific preprocessor defines.
  
  // These should all be defined in the preprocessor according to the
  // current language configuration.

  if (LangOpts.GNUMode)
    DefineBuiltinMacro(Buf, "__null=0");
  DefineBuiltinMacro(Buf, "__STDC__=1");
  if (LangOpts.AsmPreprocessor)
    DefineBuiltinMacro(Buf, "__ASSEMBLER__=1");
  if (LangOpts.C99 && !LangOpts.CPlusPlus)
    DefineBuiltinMacro(Buf, "__STDC_VERSION__=199901L");
  else if (0) // STDC94 ?
    DefineBuiltinMacro(Buf, "__STDC_VERSION__=199409L");

  // Standard conforming mode?
  if (!LangOpts.GNUMode)
    DefineBuiltinMacro(Buf, "__STRICT_ANSI__=1");
  
  if (LangOpts.CPlusPlus0x)
    DefineBuiltinMacro(Buf, "__GXX_EXPERIMENTAL_CXX0X__");

  if (LangOpts.Freestanding)
    DefineBuiltinMacro(Buf, "__STDC_HOSTED__=0");
  else
    DefineBuiltinMacro(Buf, "__STDC_HOSTED__=1");
  
  // darwin_constant_cfstrings controls this. This is also dependent
  // on other things like the runtime I believe.  This is set even for C code.
  DefineBuiltinMacro(Buf, "__CONSTANT_CFSTRINGS__=1");
  
  if (LangOpts.Blocks) {
    DefineBuiltinMacro(Buf, "__block=__attribute__((__blocks__(byref)))");
    DefineBuiltinMacro(Buf, "__BLOCKS__=1");
  }
  
  if (LangOpts.CPlusPlus) {
    DefineBuiltinMacro(Buf, "__cplusplus=1");
    DefineBuiltinMacro(Buf, "__DEPRECATED=1");
    DefineBuiltinMacro(Buf, "__EXCEPTIONS=1");
    DefineBuiltinMacro(Buf, "__GNUG__=4");
    DefineBuiltinMacro(Buf, "__GXX_WEAK__=1");
    DefineBuiltinMacro(Buf, "__private_extern__=extern");
    DefineBuiltinMacro(Buf, "_BEGIN_STD_C=extern \"C\" {");
    DefineBuiltinMacro(Buf, "_END_STD_C=}");
  } else {
    DefineBuiltinMacro(Buf, "_BEGIN_STD_C=");
    DefineBuiltinMacro(Buf, "_END_STD_C=");
  }
  
  if (LangOpts.Optimize)
    DefineBuiltinMacro(Buf, "__OPTIMIZE__=1");
  if (LangOpts.OptimizeSize)
    DefineBuiltinMacro(Buf, "__OPTIMIZE_SIZE__=1");
    
  // Initialize target-specific preprocessor defines.
  
  // Define type sizing macros based on the target properties.
  unsigned IntMaxWidth;
  const char *IntMaxSuffix;
  if (TI.getIntMaxType() == TargetInfo::LongLong) {
    IntMaxWidth = TI.LongLongWidth();
    IntMaxSuffix = "LL";
  } else if (TI.getIntMaxType() == TargetInfo::Long) {
    IntMaxWidth = TI.LongWidth();
    IntMaxSuffix = "L";
  } else {
    assert(TI.getIntMaxType() == TargetInfo::Int);
    IntMaxWidth = TI.IntWidth();
    IntMaxSuffix = "";
  }
  
  // Define type sizes. Used in <limits.h>.
  sprintf(MacroBuf, "__CHAR_BIT__=%d", (int)TI.CharWidth());
  DefineBuiltinMacro(Buf, MacroBuf);
  DefineTypeSize("__SCHAR", TI.CharWidth(), "", true, Buf);
  DefineTypeSize("__UCHAR", TI.CharWidth(), "", false, Buf);
  DefineTypeSize("__CHAR", TI.CharWidth(), "", TI.isCharSigned(), Buf, true);
  DefineTypeSize("__SHRT", TI.ShortWidth(), "", true, Buf);
  DefineTypeSize("__USHRT", TI.ShortWidth(), "", false, Buf);
  DefineTypeSize("__INT", TI.IntWidth(), "", true, Buf);
  DefineTypeSize("__UINT", TI.IntWidth(), "U", false, Buf);
  DefineTypeSize("__LONG", TI.LongWidth(), "L", true, Buf);
  DefineTypeSize("__ULONG", TI.LongWidth(), "UL", false, Buf);
  DefineTypeSize("__LLONG", TI.LongLongWidth(), "LL", true, Buf);
  DefineTypeSize("__ULLONG", TI.LongLongWidth(), "ULL", false, Buf);

  // Various types. Used in <stddef.h>.
  DefineType("__WCHAR_TYPE__", TI.getWCharType(), Buf);
  DefineType("__SIZE_TYPE__", TI.getSizeType(), Buf);
  DefineType("__PTRDIFF_TYPE__", TI.getPtrDiffType(0), Buf);

  // FIXME: TargetInfo hookize __WINT_TYPE__.
  DefineBuiltinMacro(Buf, "__WINT_TYPE__=int");

  DefineType("__INTPTR_TYPE__", TI.getIntPtrType(), Buf);
  DefineType("__UINTPTR_TYPE__", TI.getUIntPtrType(), Buf);
 
  // Wide char type. Used in <wchar.h>.
  DefineTypeSize("__WCHAR", TI.WCharWidth(), "", true, Buf);

  DefineTypeSize("__INTMAX", IntMaxWidth, IntMaxSuffix, true, Buf);
  DefineType("__INTMAX_TYPE__", TI.getIntMaxType(), Buf);
  DefineType("__UINTMAX_TYPE__", TI.getUIntMaxType(), Buf);

  
  DefineFloatMacros(Buf, "FLT", &TI.getFloatFormat());
  DefineFloatMacros(Buf, "DBL", &TI.getDoubleFormat());
  DefineFloatMacros(Buf, "LDBL", &TI.getLongDoubleFormat());

  // Define a __POINTER_WIDTH__ macro for stdint.h.
  sprintf(MacroBuf, "__POINTER_WIDTH__=%d", (int)TI.getPointerWidth(0));
  DefineBuiltinMacro(Buf, MacroBuf);
  
  if (!TI.isCharSigned())
    DefineBuiltinMacro(Buf, "__CHAR_UNSIGNED__");  

  // Define fixed-sized integer types for stdint.h
  assert(TI.CharWidth() == 8 && "unsupported target types");
  assert(TI.ShortWidth() == 16 && "unsupported target types");
  DefineBuiltinMacro(Buf, "__INT8_TYPE__=char");
  DefineBuiltinMacro(Buf, "__INT16_TYPE__=short");
  
  if (TI.IntWidth() == 32)
    DefineBuiltinMacro(Buf, "__INT32_TYPE__=int");
  else {
    assert(TI.getLongLongWidth() == 32 && "unsupported target types");
    DefineBuiltinMacro(Buf, "__INT32_TYPE__=long long");
  }
  
  // 16-bit targets doesn't necessarily have a 64-bit type.
  if (TI.LongLongWidth() == 64)
    DefineBuiltinMacro(Buf, "__INT64_TYPE__=long long");
  
#if RICH
  // Why would this be needed?
  // Add __builtin_va_list typedef.
  {
    const char *VAList = TI.getVAListDeclaration();
    Buf.insert(Buf.end(), VAList, VAList+strlen(VAList));
    Buf.push_back('\n');
  }
#endif
  
  if (const char *Prefix = TI.getUserLabelPrefix()) {
    sprintf(MacroBuf, "__USER_LABEL_PREFIX__=%s", Prefix);
    DefineBuiltinMacro(Buf, MacroBuf);
  }
  
  // Build configuration options.  FIXME: these should be controlled by
  // command line options or something.
  DefineBuiltinMacro(Buf, "__FINITE_MATH_ONLY__=0");

  if (LangOpts.Static)
    DefineBuiltinMacro(Buf, "__STATIC__=1");
  else
    DefineBuiltinMacro(Buf, "__DYNAMIC__=1");

  if (LangOpts.GNUInline)
    DefineBuiltinMacro(Buf, "__GNUC_GNU_INLINE__=1");
  else
    DefineBuiltinMacro(Buf, "__GNUC_STDC_INLINE__=1");

  if (LangOpts.NoInline)
    DefineBuiltinMacro(Buf, "__NO_INLINE__=1");

  if (unsigned PICLevel = LangOpts.PICLevel) {
    sprintf(MacroBuf, "__PIC__=%d", PICLevel);
    DefineBuiltinMacro(Buf, MacroBuf);

    sprintf(MacroBuf, "__pic__=%d", PICLevel);
    DefineBuiltinMacro(Buf, MacroBuf);
  }

  // Macros to control C99 numerics and <float.h>
  DefineBuiltinMacro(Buf, "__FLT_EVAL_METHOD__=0");
  DefineBuiltinMacro(Buf, "__FLT_RADIX__=2");
  DefineBuiltinMacro(Buf, "__FLT_ROUNDS__=1");
  sprintf(MacroBuf, "__DECIMAL_DIG__=%d",
          PickFP(&TI.getLongDoubleFormat(), -1/*FIXME*/, 17, 21, 33));
  DefineBuiltinMacro(Buf, MacroBuf);
  
  // Get other target #defines.
  TI.getTargetDefines(LangOpts, Buf);
}

/// InitializePreprocessor - Initialize the preprocessor getting it and the
/// environment ready to process a single file. This returns true on error.
///
bool InitializePreprocessor(Preprocessor &PP,
                            const PreprocessorInitOptions& InitOpts) {
  std::vector<char> PredefineBuffer;
  
  const char *LineDirective = "# 1 \"<built-in>\" 3\n";
  PredefineBuffer.insert(PredefineBuffer.end(),
                         LineDirective, LineDirective+strlen(LineDirective));
  
  // Install things like __POWERPC__, __GNUC__, etc into the macro table.
  InitializePredefinedMacros(PP.getTargetInfo(), PP.getLangOptions(),
                             PredefineBuffer);
  
  // Add on the predefines from the driver.  Wrap in a #line directive to report
  // that they come from the command line.
  LineDirective = "# 1 \"<command line>\" 1\n";
  PredefineBuffer.insert(PredefineBuffer.end(),
                         LineDirective, LineDirective+strlen(LineDirective));

  // Process #define's and #undef's in the order they are given.
  for (PreprocessorInitOptions::macro_iterator I = InitOpts.macro_begin(),
       E = InitOpts.macro_end(); I != E; ++I) {
    if (I->second)  // isUndef
      UndefineBuiltinMacro(PredefineBuffer, I->first.c_str());
    else
      DefineBuiltinMacro(PredefineBuffer, I->first.c_str());
  }

  // If -imacros are specified, include them now.  These are processed before
  // any -include directives.
  for (PreprocessorInitOptions::imacro_iterator I = InitOpts.imacro_begin(),
       E = InitOpts.imacro_end(); I != E; ++I)
    AddImplicitIncludeMacros(PredefineBuffer, *I);

  // Process -include directives.
  for (PreprocessorInitOptions::include_iterator I = InitOpts.include_begin(),
       E = InitOpts.include_end(); I != E; ++I) {
    if (I->second) // isPTH
      AddImplicitIncludePTH(PredefineBuffer, PP, I->first);
    else
      AddImplicitInclude(PredefineBuffer, I->first);
 }

  LineDirective = "# 2 \"<built-in>\" 2 3\n";
  PredefineBuffer.insert(PredefineBuffer.end(),
                         LineDirective, LineDirective+strlen(LineDirective));

  // Null terminate PredefinedBuffer and add it.
  PredefineBuffer.push_back(0);
  PP.setPredefines(&PredefineBuffer[0]);

  // Once we've read this, we're done.
  return false;
}

} // namespace ellcc
