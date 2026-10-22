
#include "PhpErrorData.h"

#include <main/php.h>
#include <Zend/zend_alloc.h>
#include <Zend/zend_builtin_functions.h>
#include <Zend/zend_types.h>
#include <Zend/zend_variables.h>

#include <string>
#include <string_view>

namespace elasticapm::php {
PhpErrorData::PhpErrorData(int type, std::string_view fileName, uint32_t lineNumber, std::string_view message) : type_(type), fileName_(fileName), lineNumber_(lineNumber), message_(message) {
    ZVAL_UNDEF(&stackTrace_);
    zend_fetch_debug_backtrace(&stackTrace_, /* skip_last */ 0, /* options */ 0, /* limit */ 0);
}

PhpErrorData::~PhpErrorData() {
    zval_ptr_dtor(&stackTrace_);
}

void PhpErrorData::discardStackTraceWithoutDestroying() {
    // Used after the executor has shut down: the stack trace may reference objects from the already freed objects store,
    // so destroying it would write to freed memory. Its memory is in the request pool and is freed with it.
    ZVAL_UNDEF(&stackTrace_);
}

int PhpErrorData::getType() const {
    return type_;
}

std::string_view PhpErrorData::getFileName() const {
    return fileName_;
}

int PhpErrorData::getLineNumber() const {
    return lineNumber_;
}

std::string_view PhpErrorData::getMessage() const {
    return message_;
}

zval *PhpErrorData::getStackTrace()  {
    return &stackTrace_;
}

}
