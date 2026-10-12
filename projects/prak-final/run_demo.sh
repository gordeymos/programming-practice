#!/bin/bash
# Запуск демонстрации программы через Docker (работает на Mac M1/M2/M3)
# Использование: bash run_demo.sh

set -e

DIR="$(cd "$(dirname "$0")" && pwd)"

echo "======================================================"
echo " Лабораторная №6, вариант 10 — Москвин Г.Е."
echo " Запуск через Docker (Linux x86-64 + 32-bit multilib)"
echo "======================================================"
echo ""

docker run -it --rm \
  --platform linux/amd64 \
  -v "$DIR":/lab \
  -w /lab \
  ubuntu:22.04 bash -c '
    echo ">>> Установка инструментов..."
    apt-get update -qq && apt-get install -y -qq gcc gcc-multilib nasm make 2>/dev/null
    echo ""

    echo "=============================="
    echo " Сборка: метод Ньютона"
    echo "=============================="
    make clean 2>/dev/null || true
    make METHOD=NEWTON
    echo ""

    echo ">>> ./a.out (площадь при трёх eps):"
    ./a.out
    echo ""

    echo ">>> ./a.out --roots (точки пересечения):"
    ./a.out --roots
    echo ""

    echo ">>> ./a.out --iter (число итераций):"
    ./a.out --iter
    echo ""

    echo ">>> ./a.out --test-root:"
    ./a.out --test-root
    echo ""

    echo ">>> ./a.out --test-integral:"
    ./a.out --test-integral
    echo ""

    echo "=============================="
    echo " Пересборка: комбинированный метод"
    echo "=============================="
    make clean
    make METHOD=COMBINED
    echo ""

    echo ">>> ./a.out (площадь, комбинированный метод):"
    ./a.out
    echo ""

    echo ">>> ./a.out --test-root:"
    ./a.out --test-root
    echo ""

    echo ">>> ./a.out --help:"
    ./a.out --help
'
