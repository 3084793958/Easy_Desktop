#!/bin/bash
set -e

# ---------- 配置 ----------
WORKDIR="${HOME}/deepin-dtk-build"
DTK_VERSION="5.6.12"          # 目标版本，PPA 中可能更新，但名称一致

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

# ---------- 检测系统发行版 ----------
detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        if [[ "$ID" == "deepin" || "$ID" == "uos" || "$NAME" =~ "Deepin" || "$NAME" =~ "UOS" ]]; then
            echo "deepin"
        elif [[ "$ID" == "ubuntu" || "$ID" == "debian" ]]; then
            echo "debian"
        else
            echo "unknown"
        fi
    else
        echo "unknown"
    fi
}

DISTRO=$(detect_distro)
echo -e "${GREEN}发行版: $DISTRO${NC}"

# ---------- 安装通用依赖 ----------
install_common_deps() {
    echo -e "${GREEN}安装通用构建依赖...${NC}"
    sudo apt update
    sudo apt install build-essential qtbase5-dev qttools5-dev-tools qtmultimedia5-dev libqt5svg5-dev libqt5x11extras5-dev libqt5charts5-dev libqt5pdf5 libqt5pdfwidgets5 qtpdf5-dev libqt5pdf5 libx11-dev libxtst-dev libpulse-dev librsvg2-dev libglib2.0-dev libgdk-pixbuf2.0-dev libcairo2-dev libgl1-mesa-dev
}

# ---------- 安装 Qt PDF 开发包 ----------
install_pdf_deps() {
    echo -e "${GREEN}安装 Qt PDF 相关依赖...${NC}"
    # 尝试安装开发包
    if sudo apt install qtpdf5-dev libqt5pdf5-dev 2>/dev/null; then
        echo -e "${GREEN}Qt PDF 开发包安装成功。${NC}"
    else
        echo -e "${YELLOW}无法通过包管理器安装 Qt PDF 开发包。${NC}"
        echo -e "${YELLOW}如果不需要 PDF 功能，可忽略；如需 PDF，请手动编译 Qt PDF 模块。${NC}"
    fi
    # 运行时库（可选）
    sudo apt install libqt5pdf5 libqt5pdfwidgets5 2>/dev/null || true
}

# ---------- 安装 DTK (优先 PPA，失败则源码编译) ----------
install_dtk() {
    if [[ "$DISTRO" == "deepin" ]]; then
        echo -e "${GREEN}在 Deepin/UOS 上通过官方源安装 DTK...${NC}"
        sudo apt install libdtkwidget-dev libdtkgui-dev libdtkcore-dev
        return 0
    fi

    # 对于 Debian/Ubuntu，先尝试 PPA 方式 (ubuntudde-dev/stable)
    if [[ "$DISTRO" == "debian" ]]; then
        echo -e "${GREEN}尝试通过 PPA (ubuntudde-dev/stable) 安装 DTK...${NC}"
        # 添加 PPA 并更新
        if sudo add-apt-repository ppa:ubuntudde-dev/stable 2>/dev/null; then
            sudo apt update
            # 尝试安装指定版本（如果 PPA 提供，否则安装最新）
            if sudo apt install libdtkwidget-dev libdtkgui-dev libdtkcore-dev; then
                echo -e "${GREEN}通过 PPA 成功安装 DTK。${NC}"
                return 0
            else
                echo -e "${YELLOW}通过 PPA 安装 DTK 失败，将回退到源码编译。${NC}"
            fi
        else
            echo -e "${YELLOW}添加 PPA 失败，将回退到源码编译。${NC}"
        fi
    fi

    # 回退方案：从源码编译 DTK
    echo -e "${YELLOW}正在从源码编译 DTK (版本 $DTK_VERSION)。这可能需要较长时间...${NC}"
    mkdir -p "$WORKDIR"
    cd "$WORKDIR"

    # 克隆并编译 dtkcore
    if [ ! -d "dtkcore" ]; then
        git clone --depth 1 --branch $DTK_VERSION https://github.com/linuxdeepin/dtkcore.git || \
        git clone --depth 1 https://github.com/linuxdeepin/dtkcore.git
    fi
    cd dtkcore
    git checkout $DTK_VERSION 2>/dev/null || git checkout master
    mkdir -p build && cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release
    make -j$(nproc)
    sudo make install
    cd ../..

    # 克隆并编译 dtkgui
    if [ ! -d "dtkgui" ]; then
        git clone --depth 1 --branch $DTK_VERSION https://github.com/linuxdeepin/dtkgui.git || \
        git clone --depth 1 https://github.com/linuxdeepin/dtkgui.git
    fi
    cd dtkgui
    git checkout $DTK_VERSION 2>/dev/null || git checkout master
    mkdir -p build && cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release
    make -j$(nproc)
    sudo make install
    cd ../..

    # 克隆并编译 dtkwidget
    if [ ! -d "dtkwidget" ]; then
        git clone --depth 1 --branch $DTK_VERSION https://github.com/linuxdeepin/dtkwidget.git || \
        git clone --depth 1 https://github.com/linuxdeepin/dtkwidget.git
    fi
    cd dtkwidget
    git checkout $DTK_VERSION 2>/dev/null || git checkout master
    mkdir -p build && cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release \
             -DDTK_VERSION=$DTK_VERSION
    make -j$(nproc)
    sudo make install
    cd ../..

    sudo ldconfig
    echo -e "${GREEN}DTK 编译安装完成。${NC}"
}

# ---------- 主流程 ----------
main() {
    install_common_deps
    install_pdf_deps
    install_dtk
    echo -e "${GREEN}所有依赖已处理完毕。${NC}"
}

main