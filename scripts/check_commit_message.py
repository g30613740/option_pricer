#!/usr/bin/env python3
"""Check the mechanically verifiable parts of the commit message policy."""

import re
import sys
from pathlib import Path


def violations(message: str) -> list[str]:
    errors = []
    if not message.strip() or not re.search(r"[A-Za-z]", message):
        errors.append("Write a nonempty commit message in English.")
    if re.search(r"[\u0400-\u052f]", message):
        errors.append("Use English; Cyrillic characters are not allowed.")
    if re.search(r"\b(?:Claude|Anthropic|Codex|OpenAI|ChatGPT|AI|ИИ)\b", message, re.I):
        errors.append("Remove references to prohibited tools or authorship.")
    if re.search(r"^\s*Co-Authored-By\s*:", message, re.I | re.M):
        errors.append("Remove co-author trailers.")
    return errors


def main() -> int:
    if len(sys.argv) != 2:
        print("Usage: check_commit_message.py MESSAGE_FILE", file=sys.stderr)
        return 2
    try:
        message = Path(sys.argv[1]).read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        print(f"Cannot read commit message: {error}", file=sys.stderr)
        return 2
    errors = violations(message)
    for error in errors:
        print(error, file=sys.stderr)
    return int(bool(errors))


if __name__ == "__main__":
    sys.exit(main())
