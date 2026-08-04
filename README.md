# The Crime Scene — CTF Web Challenge

A narrative, murder-mystery-themed web challenge built for a college Capture The Flag event. Players work through a chain of clues spread across two "case file" pages, ending with a flag revealed via a Netlify serverless function.

## Story

You're investigating a crime. The first page, **The Crime Scene**, asks for a single missing word from a witness's statement. Get it right, and you're pointed toward the second page, **The Interrogation**, where a compiled WebAssembly module — the "lie detector" — expects a specific hex sequence before it will hand over a confession.

## Tech Stack

- Static HTML/CSS/JS front end (no build tooling, no `package.json` — deployed as-is)
- WebAssembly modules (`.wasm`) compiled from C via [Emscripten](https://emscripten.org/), used as client-side logic gates
- [Netlify](https://www.netlify.com/) for static hosting + a serverless function (`netlify/functions/get_flag.js`)
- `crypto.subtle` (SubtleCrypto/WebCrypto) in-browser for hashing

## Project Structure

```
index.html              Page 1 — "The Crime Scene"
page2.html               Page 2 — "The Interrogation"

logic_page1.c            Source for page 1's Wasm module
logic_page1.js            Emscripten JS glue for page 1's Wasm module
logic_page1.wasm          Compiled Wasm binary for page 1

logic_page2.c             Source for page 2's Wasm module ("the lie detector")
logic_page2.js             Emscripten JS glue for page 2's Wasm module
logic_page2.wasm            Compiled Wasm binary for page 2

netlify/functions/get_flag.js   Serverless function that releases a clue/flag on request
netlify.toml                     Netlify build/publish configuration
alternatives.txt                 Design notes / discarded red herrings considered while building the challenge
```

## How It Works (high level)

1. **Page 1** validates a player-submitted word client-side by hashing it and comparing against a stored digest. A correct submission calls the Netlify function to fetch the next clue and unlocks a link to page 2.
2. **Page 2** loads a compiled Wasm module and passes a player-submitted hex value into an exported function. The module's logic (XOR-based) determines success or failure, and on success decrypts and reveals a key string in memory.
3. Clues along the way point toward encoding schemes (Morse code, Base64) and highlight mismatches between a file's *declared* comment/intent and its *actual* compiled behavior — a deliberate red herring built into the C source.

No solution details are included here by design, to keep this README safe to share with participants.

## Running Locally

This is a static site with one serverless function, so any Netlify-compatible local dev flow works:

```bash
npm install -g netlify-cli
netlify dev
```

This serves `index.html` / `page2.html` from the project root and runs `netlify/functions/get_flag.js` locally, matching the `netlify.toml` configuration (`publish = "."`, `functions = "netlify/functions"`).

Alternatively, serve the root directory with any static file server (e.g. `npx serve .`) for front-end-only testing — note that the "fetch clue" step on page 1 requires the Netlify function, so that part won't work without `netlify dev` or a live deployment.

## Deployment

Deployed on Netlify directly from this repository. `netlify.toml` sets:
- `publish = "."` — site root is the publish directory
- `functions = "netlify/functions"` — serverless functions live here

## Rebuilding the Wasm Modules

The `.c` files are the original sources; the `.js`/`.wasm` pairs are their Emscripten build output. To rebuild after editing a `.c` file (requires the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html)):

```bash
emcc logic_page1.c -o logic_page1.js -s EXPORTED_FUNCTIONS="['_get_encrypted_flag_ptr']" -s EXPORTED_RUNTIME_METHODS="['ccall','cwrap']"
emcc logic_page2.c -o logic_page2.js -s EXPORTED_FUNCTIONS="['_process_and_check','_get_key_ptr']" -s EXPORTED_RUNTIME_METHODS="['ccall','cwrap']"
```

(Adjust exported function flags to match whatever the current `.c` source actually exports.)
