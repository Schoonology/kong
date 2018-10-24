#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

SHELL_ARGS=$*
PROGRESS="--out-format=%i %f"
TARGET_HOST=kong.local
TARGET="pi@$TARGET_HOST"
TUB_NAME="$(date -u +"%Y-%m-%dT%H:%M:%SZ")"

pull() {
  rsync -a $PROGRESS $SHELL_ARGS "$@"
}

push() {
  rsync -au $PROGRESS $SHELL_ARGS "$@"
}

if ! ping -c 1 "$TARGET_HOST" &> /dev/null; then
  if [ -t 1 ]; then
    echo "Cannot reach Donkey Car ($TARGET_HOST). Skipping sync."
  fi
  exit 1
fi

cd "$(dirname "${BASH_SOURCE[0]}")/.."

push car/*.py car/parts $TARGET:~/kong/.
push config/wpa_supplicant.conf $TARGET:/tmp/wpa_supplicant.conf
ssh $TARGET 'sudo cp /tmp/wpa_supplicant.conf /etc/wpa_supplicant/wpa_supplicant.conf'

mkdir -p models scratch tubs
pull --remove-source-files $TARGET:~/kong/tub/ tubs/$TUB_NAME/
rmdir tubs/$TUB_NAME &> /dev/null || true
push --delete models/ $TARGET:~/kong/models/

echo "Sync successful."
