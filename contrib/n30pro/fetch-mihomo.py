#!/usr/bin/env python3
from pathlib import Path
import gzip, hashlib, json, urllib.request
root = Path(__file__).resolve().parents[2]
lock = json.loads((Path(__file__).parent/'source-lock.json').read_text())['mihomo']
with urllib.request.urlopen(lock['download_url'], timeout=120) as response:
    archive = response.read()
assert hashlib.sha256(archive).hexdigest() == lock['gzip_sha256']
binary = gzip.decompress(archive)
assert hashlib.sha256(binary).hexdigest() == lock['elf_sha256']
dest = root/'package/local/mihomo/files/clash_meta'
dest.parent.mkdir(parents=True, exist_ok=True)
dest.write_bytes(binary)
dest.chmod(0o755)
print('Verified official Mihomo ARM64 core installed into source tree')
