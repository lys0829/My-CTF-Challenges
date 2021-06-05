# AIS3 PreExam/MyFirstCTF 2021

## WriteMe
* Category: Pwn
* Difficulty: Easy
* Solves: 61/327 (PreExam), 3/190 (MyFirstCTF)
* Keywords: Lazy Binding, GOT Hijacking

checksec:
```
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
```
這題做的事情是先把 `system` 的 GOT 寫成 0，然後給一次任意寫入 8 bytes 的機會，最後幫你 call `system("/bin/sh")`。
從 `checksec` 可以發現是 `Partial RELRO`，再加上任意寫入，明顯是 GOT Hijacking 套路，就是要把 `system` 的 GOT 寫掉。
不過以往的 GOT Hijacking 大家可能會知道就寫成 one gadget 或者是想要 call 的 function 的 plt 就行了，但這題如果把 GOT 寫入 `system` 的 plt address 會發現行不通，可是如果寫成其他的 function 例如 puts 的 plt 會發現可以正常運作。原因是因為實際上 call function 的行為是 call 那個 function 的 plt，然後那段 plt 會去查 GOT 表，跳到 GOT 表所寫的位置，所以當你把 `system` 的 GOT 填入 `system` 的 plt 時就會發生無窮 loop。
而這題是希望考大家知不知道 lazy binding 實際上怎麼運作的，而不是只知道蓋 GOT 可以亂跳。
實際上 lazy binding 的運作是，一開始 GOT 上指的地方會是一個 resolve function，所以在第一次 call 到那個 library function 時，會先執行 GOT 上指的那個 resolve function，找出那個 function 的實際位置後寫回 GOT 然後再執行那個 function，往後再 call 到同樣 function 時，因為 GOT 上填的已經是那個 function 的實際位置了，所以就會直接執行。看到這邊會發現，要不要執行 resolve 其實沒有其他條件判斷，而是 GOT 上剛開始指得就是 resolve function，所以會執行。
所以這題的解法就是找到 `system` 的 resolve function，然後填回 GOT，這樣下次 call `system` 時就可以再重新找一次位置。至於這個 resolve function 的位置，以前會是該 function 的 plt+6，不過比較新的 compiler 把所有 function 的 resolve function 額外拉出來變成另外一段。
總結來說，這題就是在 Address 填 `0x404028`，Value 填 `0x401050`。不過由於答案是固定的，所以其實真的不知道要填啥也可以爆搜XD
另外，這題為了防止有人寫成 main 來無限次任意寫，所以還有把 scanf 破壞掉。

## Blind
* Category: Misc
* Difficulty: Easy
* Solves: 62/327 (PreExam), 3/190 (MyFirstCTF)
* Keywords: x86-64 system call, dup

這題把 stdout 關掉後，讓你 call 一個任意 syscall，然後將 flag 輸出到 stdout。
目標應該很明顯，就是想辦法透過一個 syscall 讓輸出到 stdout 的內容可以被我們看到。
翻一翻 x86-64 syscall table 就可以發現 `dup` 跟 `dup2` 兩個 syscall，應該大多數人都是 `dup2(2,1)`，不過其實 `dup(2)` 也可以，因為 `dup` 會找最小可用的 fd，於是就會用 1。
至於 dup 在幹嘛，大家可以自己去查一下囉。

## \[震撼彈\] AIS3 官網疑遭駭！
* Category: Misc
* Difficulty: Easy
* Solves: 44/327 (PreExam), 2/190 (MyFirstCTF)
* Keywords: forensics, pcap, http, virtual host, webshell

打開封包會發現一堆 http request，都是 request `/index.php?page=bHMgLg%3d`，仔細看的話會發現其中有一包是 `/Index.php?page=%3DogLgMHb`，而且 response 內容不太一樣。
Response:
```
Index.php
index.php
```
看到這個可以聯想到是不是 ls 指令的結果，再仔細觀察 page 參數會發現他是個 base64 倒過來得字串(從開頭是"="可以猜得出來)，而將其 base64 decode 之後會發現是 `ls .`，從這裡就可以看得出他應該是個 webshell。
除了這包之外會發現沒有其他可疑封包了，從這是個 Webshell 而且這個 IP 在這次的比賽網段以及後來放的提示應該可以猜出必須實際去戳這個 Webshell。
然而當你去戳 `http://10.153.11.126:8100/Index.php` 時會發現 404 Not Found，這是這題埋的另一個雷，回去仔細觀察 `/Index.php?page=%3DogLgMHb` 的 HTTP Header:
```
GET /Index.php?page=%3DogLgMHb HTTP/1.1
Host: magic.ais3.org:8100
User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:88.0) Gecko/20100101 Firefox/88.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Connection: keep-alive
Upgrade-Insecure-Requests: 1
Cache-Control: max-age=0, no-cache
Pragma: no-cache
```
會看到 `Host: magic.ais3.org:8100`，這裡用到了 virtual host 的技術，讓同一個 http service 可以根據 domain name 將網站分開，可是 `dig magic.ais3.org` 會發現根本沒這個 domain name，這邊有兩個方法，一個是自己送 HTTP Request，並且加上 `Host: magic.ais3.org:8100` 這個 Header，或者是改 `/etc/hosts`，自己加上 domain name 對應。
成功戳到 Webshell 之後，flag 就放在根目錄。
附上一行解 (by seadog007): `curl -H 'Host: magic.ais3.org:8100' "10.153.11.126:8100/Index.php?page=\`echo cat ../fl\*| base64 | rev\`"`

這題的解題情況比我預期的慘很多QQ 我以為大家都會用 wireshark 而且也知道 virtual host 的說。

## AIS3Shell
* Category: Misc
* Difficulty: Medium
* Solves: 11/327 (PreExam), 0/190 (MyFirstCTF)
* Keywords: heap overflow

這題提供了一個假 shell 服務，可以定義自己的指令，並且執行，不過指令有限制，只能 `echo`,`ls`,`sl`，所以沒辦法 get shell 也沒辦法直接哪 flag。
這題的洞在自己實作的 Memory Allocator，在 Init 跟 Alloc 時的 index 相反，Init 用 `MEM[SIZE][NUM]`，Alloc 取的時候卻變成 `MEM[NUM][SIZE]`，也就是說實際上拿到的 chunk size 會比較小，這時就可以 overflow。
Exploit 方法是先定義一個正常的指令，然後在定義另一個指令的 name 時，利用這個 overflow 把前面那個正常的指令蓋掉，蓋成 `/bin/sh` 之類的。
[exploit.py](ais3shell/exploit.py)
