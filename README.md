## BencodeUtils path `lib/bencodeUtil`

### Feautures

- encode/decode int  `23` <--> `i23e`
- encode/decode string   `hello` <--> `5:hello`
- encode/decode list   `[23,"hello",476]` <--> `li23e5:helloi476ee`
- encode/decode dict   `{"key":"val","key2":52}` <--> `d3:key3:val`

## SOURCES (links)

- https://wiki.theory.org/BitTorrentSpecification#Purpose
- https://en.wikipedia.org/wiki/Bencode
- https://www.bittorrent.org/beps/bep_0003.html
- [Build torrent on Go](https://blog.jse.li/posts/torrent/)