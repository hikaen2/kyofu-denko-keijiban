# 恐怖電光掲示板

昔書いた電光掲示板のプログラムです。
H8/3048マイコン用です。

## ファイル一覧
- h8rom.x : リンカスクリプト
- h8crt0.s : スタートアップルーチン
- main.c
- public.h : グローバル変数
- shell.h, shell.c
- timer.h, timer.c
- sci.h, sci.c, sci_printf.c : serial communication interface
- command.h, command_def.h, command.c
- bmp.h, bmp_def.h, bmp.c
