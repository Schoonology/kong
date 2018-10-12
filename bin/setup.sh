#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

TARGET_HOST=kong.local
TARGET="pi@$TARGET_HOST"

ssh-copy-id -i ~/.ssh/id_rsa $TARGET
