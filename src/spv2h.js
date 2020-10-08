const fs = require('fs');

const [ src_file, dst_file, name ] = process.argv.slice(2);

fs.writeFileSync(dst_file, `const uint32_t ${ name }[] = { ${ new Uint32Array(new Uint8Array(fs.readFileSync(src_file)).buffer) } };\n`);
