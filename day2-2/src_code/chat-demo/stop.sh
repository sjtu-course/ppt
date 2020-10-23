#!/bin/bash
# 停止后端
pkill -f target/demo
# 停止前端
ps aux | grep -E "(.*)node(.*)vue-cli-service serve --mode(.*)" | grep -v "grep" | awk '{print $2}' | xargs kill