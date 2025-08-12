#include <emscripten.h>

// The Base64 encoded flag for "CTF{The_Victim_Deserved_It}"
// This is now hidden inside the Wasm module.
char encrypted_flag[] = "ZmxhZ3tIZSB3YXNuJ3QgdGhlIG9ubHkgb25lfQ";

// This function just returns the memory address of the string.
EMSCRIPTEN_KEEPALIVE
char* get_encrypted_flag_ptr() {
    return encrypted_flag;
}
