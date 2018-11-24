# PageTableWalk と Page Fault の関係
本ページでは、Linuxにおける *PageTableEntry* 機構により発生する *Page Table Walk* と、  
*Demand Paging* 機構による、*Page Fault* の違いについて解説する.  

### OSレイヤ
1. Linuxでは、仮想メモリと物理メモリの２層のメモリ構成をしており、  
通常ユーザーランドから使用されるメモリアドレスは仮想アドレス（論理アドレス/logical adress)である.  
仮想メモリは実際に必要になった時のみ物理アドレスに紐づけられる、 
逆に言うと使われていない時は、物理アドレスに紐づけられないという性質を持つ.  
これを  *Demand Paging* 機構と呼ぶ<sup>[1](#footnote)</sup>.    
  
2. 仮想メモリは *ページ(page)* という単位で管理される。↑において、実際に物理メモリと紐づけられた論理アドレスと物理アドレスとのマッピング表がカーネル内に存在する。これは、*ページテーブルエントリ(Page Table Entry/PTE)* と呼ばれる。  
※PTEはメモリ上に格納される  

### CPUレイヤ
3. CPUは、メモリを参照する時に、↑の仮想アドレス（論理アドレス/logical adress)を参照しようとする  
※セグメント機構が動いていない前提  
  
4. ↑の処理がCPU内で発火すると、 *Memory Managemnt Unit(MMU)* と呼ばれるCPU内の物理コンポーネントが、  
CPUが見ようとした物理アドレスと論理アドレスのマッピング解決を行うためにOSレイヤで解説のページテーブルエントリ(PTE)を参照する。  
これを、ページテーブルウオーク( *PageT able Walk* )という.  
  
5. ページテーブルウオークにより発生するオーバーヘッドを削減するため、CPUは一度行われたページテーブルウオークにより得られたレコードについては  
MMU内の *Translation Lookaside Buffer(TLB)* と呼ばれるキャッシュ機構に一時的なキャッシュとして保存を行う.   
  
6. 次回以降、CPUがメモリを参照した時に、まず最初にTLBが参照される。  
以降の処理は以下に分岐する。  
- A:欲しいページテーブルエントリがあったTLB上にあった( *TLB HIT* ) →　処理終了  
- B:欲しいページテーブルエントリがあったTLB上にない( *TLB MISS* ) → ページテーブルウオーク発生  
- B-1:ページテーブルウオークの結果欲しいレコードがPTE上にあった →　結果をTLBに乗せて終わり  
- B-2:ページテーブルウオークの結果欲しいレコードがPTE上にもなかった（ *page fault* )  
      ⇒　CPU(1)の話を背景に、仮想メモリと物理メモリの再度の紐づけを行いPTEを更新、PET更新に伴いCPU上のTLBも一度クリア 

### TLBミスの発生状況の確認
`perf`コマンドを使用することでTLBミスの発生状況の確認ができる。  

```sh
# perf stat -e dTLB-loads,dTLB-stores,iTLB-load-misses,iTLB-loads -I 1000
#           time             counts unit events
     1.001978040         27,398,445      dTLB-loads
     1.001978040         14,215,811      dTLB-stores
     1.001978040             54,771      iTLB-load-misses          #    0.06% of all iTLB cache hits
     1.001978040         89,732,044      iTLB-loads
```

### 脚注
<a name="footnote">1</a>: [The Linux Kernel / 4. メモリ管理](http://archive.linux.or.jp/JF/JFdocs/The-Linux-Kernel-4.html)

### 参考文献
- [０から作るOS開発　ページングその１　ページとPTEとPDE ](http://softwaretechnique.jp/OS_Development/kernel_development07.html)

