//
//	pwError.c - Standard error handling.
//

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "pwError.h"

namespace pw {

//
// ErrorList - Error list constructor.
//
ErrorList::ErrorList()
{
    count = 0;
    messages = NULL;
    flags = 0;
}

//
// ~ErrorList - destruct an error list.
//
ErrorList::~ErrorList()
{
    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < Error::INFOCNT; ++j) {
            if (messages[i]->infoMsgs[j]) {
                for (int k = 0; k < messages[i]->count[j]; ++k)
                    delete messages[i]->infoMsgs[j][k];
                delete messages[i]->infoMsgs[j];

            }
        }
        delete messages[i];
    }

    delete messages;
}


//
// add - Add an error to an error list.
//
Error *ErrorList::add(Error::Type type, const std::string& file,
                          int startline, int startcolumn, int endline, int endcolumn,
                          const char *format, va_list ap)
{
    Error *ep = new Error;
    Error **epp;
    int i;
    char buffer[1024];	// RICH

    for (i = 0; i < Error::INFOCNT; ++i) {
        ep->count[i] = 0;
        ep->infoMsgs[i] = NULL;
    }
    ep->type = type;
    vsprintf(buffer, format, ap);
    ep->string = buffer;
    ep->file = file;
    ep->startline = startline;
    ep->startcolumn = startcolumn;
    ep->endline = endline;
    ep->endcolumn = endcolumn;
    if (flags & STDIO) {
        // Send the error immediately.
        output(stdout, ep);
        delete ep;
        // RICH: No info can be added to this error.
        return NULL;
    }

    epp = (Error **)realloc(messages, (count + 1) * sizeof(Error *));
    if (epp == NULL) {
        delete ep;
        return NULL;
    }

    epp[count] = ep;
    messages = epp;
    ++count;
    return ep;
}

//
// info - Add information to an error message.
//
bool Error::info(Error *errormsg, Error::Info which, const char *format, ...)
{
    va_list ap;

    if (errormsg == NULL) {
        return false;
    }

    va_start(ap, format);
    bool status = errormsg->info(which, format, ap);
    va_end(ap);
    return status;
}

//
// info - Add information to an error message.
//
bool Error::info(Error::Info which, const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    bool status = info(which, format, ap);
    va_end(ap);
    return status;
}

//
// info - Add information to an error message.
//
bool Error::info(Error::Info which, const char *format, va_list ap)
{
    const char **ip;
    char buffer[1024];	// RICH

    if (which >= Error::INFOCNT) {
        return false;
    }

    ip = (const char **)realloc(infoMsgs[which], (count[which] + 1) * sizeof( char *));
    if (ip == NULL) {
        return false;
    }

    infoMsgs[which] = ip;
    vsprintf(buffer, format, ap);
    infoMsgs[which][count[which]] = buffer;
    if (infoMsgs[which][count[which]] == NULL) {
        return false;
    }

    ++count[which];
    return true;
}

//
// compare - compare two error messages
//
int
Error::compare(const void *a, const void *b)
{
    const Error *first = *(const Error **)a;
    const Error *second = *(const Error **)b;
    int i;

    // Sort by file name.
    if (first->file.length() && second->file.length()) {
        i = first->file.compare(second->file);
    } else if (first->file.length() == 0 && second->file.length() == 0) {
        i = 0;                                  // Both empty.
    } else if (first->file.length()) {
        return 1;                               // First has a name.
    } else {
        return -1;                              // Second has a name.
    }

    if (i != 0) {
        return i;                               // Alphabetically by file name.
    }

    // Sort by source position.
    if (first->startline < second->startline) {
        return -1;
    }
    if (first->startline > second->startline) {
        return 1;
    }
    if (first->endline < second->endline) {
        return -1;
    }
    if (first->endline > second->endline) {
        return 1;
    }
    if (first->startcolumn < second->startcolumn) {
        return -1;
    }
    if (first->startcolumn > second->startcolumn) {
        return 1;
    }
    if (first->endcolumn < second->endcolumn) {
        return -1;
    }
    if (first->endcolumn > second->endcolumn) {
        return 1;
    }

    // Sort by error severity.
    if (first->type > second->type) {
        return 1;
    }
    if (first->type < second->type) {
        return -1;
    }
    return 0;
}

//
// sort - Sort an error list.
//
void ErrorList::sort()
{
    qsort(messages, count, sizeof(Error *), Error::compare);
}


//
// errorPosition - Format an error's position in a standard way.
//
void errorPosition(ErrorList *errorlist, std::string& buffer, const std::string& file,
              int startline, int startcolumn, int endline, int endcolumn,
              bool trailer)
{
    errorlist->position(buffer, file, startline, startcolumn, endline, endcolumn, trailer);
}

//
// position - Format an error's position in a standard way.
//
void ErrorList::position(std::string& buffer, const std::string& file,
                           int startline, int startcolumn, int endline, int endcolumn,
                           bool trailer)
{
    buffer = file;

    if (startline) {
        // have a start line
        if (buffer.length()) {
            buffer += ':';
        }

        char temp[1024];	// RICH
        sprintf(temp, "%d", startline);
        buffer += temp;
        if (!(flags & LINEONLY) && startcolumn >= 0) {
            sprintf(temp, ".%d", startcolumn);
            buffer += temp;
        }

        if (!(flags & (LINEONLY|STARTONLY)) && endline) {
            // have an end line
            sprintf(temp, "-%d", endline);
            buffer += temp;
            if (endcolumn >= 0)
                sprintf(temp, ".%d", endcolumn);
                buffer += temp;
        }
    }

    if (trailer && buffer.length()) {
        buffer += ':';
    }
}

//
// modifier - Return a modifier string.
//
const char *Error::modifier(Error::Type type) {
    switch (type) {
    case Error::WARNING:
        return "warning";
    case Error::UNDEFINED:
        return "undefined";
    case Error::INFO:
        return "information";
    case Error::FATAL:
        return "fatal error";
    case Error::INTERNAL:
        return "internal error";
    case Error::ERROR:
        return "error";
    case Error::STYLE:
        return "style";
    default:
        return "unknown";
    }
}

//
// output - Output an error message.
//
void ErrorList::output(FILE* fp, Error* ep)
{
    const char *modifier;
    std::string buffer;

    modifier = ep->modifier();
    position(buffer, ep->file, ep->startline, ep->startcolumn,
             ep->endline, ep->endcolumn, true);

    fprintf(fp, "%s %s - %s\n", buffer.c_str(), modifier, ep->string.c_str());

    // Print error information, if any.
    for (int which = 0; which < Error::INFOCNT; ++which) {
        for (int j = 0; j < ep->count[which]; ++j) {
            fprintf(fp, "%*s%s\n", (which+1)*4, " ", 
                   ep->infoMsgs[which][j]);
        }
    }
}

//
// print - Print error messages in the standard format.
//
void ErrorList::print(FILE *fp)
{
    for (int i = 0; i < count; ++i) {
        output(fp, messages[i]);
    }
}

};
