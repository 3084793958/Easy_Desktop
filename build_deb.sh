#!/bin/bash
set -e

# ========== 配置 ==========
PRO_FILE="Easy_Desktop.pro"
VERSION="1.1.0-AKA-d26.7.15"
MAINTAINER="3084793958 <3084793958@qq.com>"
DESCRIPTION="Easy Desktop - a desktop"
SECTION="utils"
PRIORITY="optional"

SOURCE_DIR="$(pwd)"
BASE_BUILD_DIR="${SOURCE_DIR}/build"
OUTPUT_DIR="${BASE_BUILD_DIR}/debs"
RELEASE_DIR="${BASE_BUILD_DIR}/releases"
mkdir -p "$OUTPUT_DIR"
mkdir -p "$RELEASE_DIR"

VARIANTS=(
    "empty:0:0:0"
    "chart:0:1:0"
    "pdf:0:0:1"
    "dtk:1:0:0"
    "chart_pdf:0:1:1"
    "dtk_chart:1:1:0"
    "dtk_pdf:1:0:1"
    "dtk_chart_pdf:1:1:1"
)

get_depends() {
    local use_dtk=$1
    local use_chart=$2
    local use_pdf=$3
    local deps=""
    [[ "$use_chart" == "1" ]] && deps+="libqt5charts5, "
    [[ "$use_pdf" == "1" ]] && deps+="libqt5pdf5, "
    [[ "$use_dtk" == "1" ]] && deps+="libdtkwidget5, libdtkgui5, libdtkcore5, "
    deps=${deps%, }
    echo "$deps"
}

build_one() {
    local suffix_under=$1
    local use_dtk=$2
    local use_chart=$3
    local use_pdf=$4

    local suffix_dash="${suffix_under//_/-}"
    local pkg_name="easy-desktop-${suffix_dash}"
    local build_dir="${BASE_BUILD_DIR}/build_${suffix_under}"
    local install_dir="${build_dir}/install_root"

    echo "=========================================="
    echo "Building: $pkg_name (DTK=$use_dtk, CHART=$use_chart, PDF=$use_pdf)"
    echo "=========================================="

    rm -rf "$build_dir"
    mkdir -p "$build_dir"
    cd "$build_dir"

    cp "${SOURCE_DIR}/${PRO_FILE}" .
    # 删除原有的 DEFINES += USE_* 行（避免冲突）
    sed -i '/DEFINES += USE_DTK/d' "$PRO_FILE"
    sed -i '/DEFINES += USE_CHART/d' "$PRO_FILE"
    sed -i '/DEFINES += USE_PDF/d' "$PRO_FILE"
    # 注意：不删除 contains 条件块，它们会依赖头部插入的 DEFINES

    # 构建要插入头部的 DEFINES
    local defines=""
    [[ "$use_dtk" == "1" ]] && defines+=" USE_DTK"
    [[ "$use_chart" == "1" ]] && defines+=" USE_CHART"
    [[ "$use_pdf" == "1" ]] && defines+=" USE_PDF"

    # 如果存在需要添加的宏，则在文件头部插入一行 DEFINES
    if [[ -n "$defines" ]]; then
        sed -i "1i\\DEFINES +=$defines" "$PRO_FILE"
    fi

    # 创建符号链接到源码目录（除了 build）
    for item in "${SOURCE_DIR}"/*; do
        base=$(basename "$item")
        [[ "$base" == "build" ]] && continue
        [[ ! -e "$base" ]] && ln -sf "$item" "$base"
    done

    qmake "$PRO_FILE" CONFIG+=release
    make -j$(nproc)

    rm -rf "$install_dir"
    mkdir -p "$install_dir/usr/local/bin"
    mkdir -p "$install_dir/usr/share/applications"

    if [[ -f "Easy_Desktop" ]]; then
        strip Easy_Desktop
    else
        echo "ERROR: Easy_Desktop not found!"
        exit 1
    fi
    cp "Easy_Desktop" "$install_dir/usr/local/bin/"

    cp "Easy_Desktop" "$RELEASE_DIR/Easy_Desktop_${suffix_dash}"

    cat > "$install_dir/usr/share/applications/easy-desktop.desktop" <<EOF
[Desktop Entry]
Name=Easy Desktop
Comment=$DESCRIPTION
Exec=/usr/local/bin/Easy_Desktop
Icon=easy-desktop
Terminal=false
Type=Application
Categories=Utility;
EOF

    mkdir -p "$install_dir/DEBIAN"
    local depends=$(get_depends "$use_dtk" "$use_chart" "$use_pdf")
    local conflicts=""
    for v in "${VARIANTS[@]}"; do
        other_suffix_under="${v%%:*}"
        other_suffix_dash="${other_suffix_under//_/-}"
        conflicts+="easy-desktop-${other_suffix_dash}, "
    done
    conflicts=${conflicts%, }

    cat > "$install_dir/DEBIAN/control" <<EOF
Package: $pkg_name
Version: $VERSION
Architecture: amd64
Maintainer: $MAINTAINER
Section: $SECTION
Priority: $PRIORITY
Depends: $depends
Conflicts: $conflicts
Description: $DESCRIPTION
 This package is built with:
 DTK=${use_dtk}, Chart=${use_chart}, PDF=${use_pdf}
 .
 Choose the variant that matches your needs.
EOF

    dpkg-deb --build "$install_dir" "${pkg_name}_${VERSION}_amd64.deb"
    mv "${pkg_name}_${VERSION}_amd64.deb" "$OUTPUT_DIR/"

    cd "$SOURCE_DIR"
    echo "Finished: $pkg_name"
    echo ""
}

for var in "${VARIANTS[@]}"; do
    IFS=':' read -r suffix use_dtk use_chart use_pdf <<< "$var"
    build_one "$suffix" "$use_dtk" "$use_chart" "$use_pdf"
done

echo "All 8 .deb packages have been created in: $OUTPUT_DIR"
ls -lh "$OUTPUT_DIR"/*.deb
