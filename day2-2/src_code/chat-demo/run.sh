#!/bin/bash
set -e

MODE=$1
if [ ! "$1" ]; then
echo "请选择启动模式: ./run.sh prod 或者 ./run.sh dev"
exit 1
fi

# 启动后端
make "${MODE}" --directory backend

# 启动前端
cd frontend
npm install
nohup npm run "${MODE}" > /dev/null 2&>1 &