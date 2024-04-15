#include "ByteArray.h"

#include "base/Types.h"

#include <errno.h>

const char ByteArray::kCRLF[] = "\r\n";

const size_t ByteArray::kCheapPrepend;
const size_t ByteArray::kInitialSize;
