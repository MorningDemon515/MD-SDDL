#!/system/bin/sh

# 1. 动态获取当前 sh 脚本所在的绝对路径
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
DEX_PATH="$SCRIPT_DIR/classes.dex"

# 2. 检查 dex 文件是否存在
if [ ! -f "$DEX_PATH" ]; then
    echo "❌ 错误: 在 $SCRIPT_DIR 下未找到 classes.dex 文件！"
    exit 1
fi

# 3. 自动提权运行 (适配普通用户与 Root 环境)
if [ "$(id -u)" -ne 0 ]; then
    echo "[!] 当前非 Root，正在请求 Root 权限..."
    exec su -c "CLASSPATH=$DEX_PATH app_process /system/bin com.snow.SnowflakeApp"
    exit
fi

export CLASSPATH=$DEX_PATH
exec app_process /system/bin com.snow.SnowflakeApp
