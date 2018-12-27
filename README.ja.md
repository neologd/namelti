# Namelti : 人名の読み仮名をカタカナで自動付与するライブラリ

Namelti は開発における初期段階です。
そのためとても不安定です。
しかしながら、あなたの製品に利用可能な品質を目指して、今後も長期に渡り改善を続けます。

## Information
2018-12-27 の状況: gRPC インターフェイスのデザインを変更したのでサーバの再実装中

## Getting started

### Memory requirements
- Required: 1.5GB of RAM
- Recommend: 5GB of RAM
    - Current maximum binary size is 1.1GB

### Dependencies
- CentOS 7
- mecab
- mecab-ipadic-NEologd

#### Examples
- On CentOS 7

    $ git clone --depth 1 https://github.com/neologd/namelti
    $ cd namelti/src
    $ make
    $ sudo make install
    $ cd exec
    $ make
    $ ./namelti_cli
    佐藤敏紀
    サトウトシノリ, サトウサトシキノ, サトウサトシオサム, サトウトシキノ

## Copyrights
Copyright (c) 2018 Toshinori Sato (@overlast) All rights reserved.

We select the 'Apache License, Version 2.0'. Please check following link.

- https://github.com/neologd/namelti/blob/master/LICENSE
