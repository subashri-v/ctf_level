
#include <emscripten.h>
#include <stdint.h>
#include <string.h> // Required for strlen()

// The XOR key for encrypting/decrypting the key_string.
// This should be different from the secret_xor_key used in process_and_check.
const uint8_t DECRYPTION_XOR_KEY = 0x5A;

// Encrypted form of "DECEPTION" (9 characters)
// Calculated as: Each_Char_Hex ^ 0x5A
// D(0x44)^0x5A=0x1E, E(0x45)^0x5A=0x1F, C(0x43)^0x5A=0x19, E(0x45)^0x5A=0x1F,
// P(0x50)^0x5A=0x0A, T(0x54)^0x5A=0x0E, I(0x49)^0x5A=0x13, O(0x4F)^0x5A=0x15,
// N(0x4E)^0x5A=0x14
char key_string[] = {
    0x1E, 0x1F, 0x19, 0x1F, 0x0A, 0x0E, 0x13, 0x15, 0x14,
    0x00 // Null terminator for the string
};

// A static flag to ensure decryption only happens once.
// This prevents re-encrypting the string if get_key_ptr is called multiple times.
static int decrypted_flag = 0;

// This function decrypts the key_string in place.
// It's called by get_key_ptr before returning the string.
void decrypt_key_string_in_place() {
    if (decrypted_flag == 0) {
        for (int i = 0; i < strlen(key_string); i++) {
            key_string[i] ^= DECRYPTION_XOR_KEY;
        }
        decrypted_flag = 1; // Mark as decrypted
    }
}

// This function returns the memory address of the key string.
// It now decrypts the string before returning its pointer.
EMSCRIPTEN_KEEPALIVE
char* get_key_ptr() {
    decrypt_key_string_in_place(); // Decrypt the string
    return key_string;
}

// This function checks the input and returns 0 on success,
// or the wrong calculated output on failure.
EMSCRIPTEN_KEEPALIVE
uint32_t process_and_check(uint32_t input_key) {
    // This is the line that has been changed
    const uint32_t secret_xor_key = 0x434F4445; // Represents "CODE"
    const uint32_t target_output = 0x666C6167; // Represents "flag{"
    uint32_t result = input_key ^ secret_xor_key;

    if (result == target_output) {
        return 0; // Success
    } else {
        return result; // Failure
    }
}

// Add a minimal main function to satisfy the linker
int main() {
    return 0;
}

