#pragma once

#define ERROR(cond, msg) assert((printf("%s", (!cond) ? msg : "\0"), cond))
