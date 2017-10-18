# Petunjuk Penggunaan Program

Penggunaan program dimulai dengan menjalankan comand :

```
./sendfile <filename> <windowsize> <buffersize> <destination_ip> <destination_port>
```

pada terminal yang ingin digunakan sebagai pengirim. Pada :
1.	<filename> diisi dengan nama file yang akan dikirim
2.	<windowsize> diisi dengan windowsize yang diinginkan
3.	<buffersize> diisi dengan besar buffer yang ingin dikirim
4.	<destination_ip> diisi dengan ip dari penerima file
5.	<destination_port> diisi dengan port yang digunakan oleh penerima untuk membangun koneksi dengan pengirim.
Setelah itu, membuka terminal lain yang ingin digunakan sebagai penerima dan menjalankan command :

```
./recvfile <filename> <windowsize> <buffersize> <port>
```

Pada :
1.	<filename> diisi dengan nama file yang ingin digunakan untuk menerima file dari pengirim
2.	<windowsize> diisi dengan windowsize yang diinginkan
3.	<buffersize> diisi dengan besar buffer yang ingin dikirim
4.	<port> diisi dengan port yang ingin digunakan untuk menerima file.

# Cara Kerja Sliding Window Protocol

Sliding window protocol adalah salah satu protocol untuk melakukan transmisi data berbasis packet. Secara konsep, setiap transmisi yang dilakukan dilengkapi dengan consecutive sequence number yang unik, dan penerima menggunakan sequence number tersebut untuk mengurutkan packet yang diterima, dimana selain itu juga menolak packet yang duplikat serta mengidentifikasi packet yang hilang. 

Sliding window protocol membuat kita dapat melakukan transfer packet dengan sequence number yang besarnya sudah ditentukan dengan tidak terbatas. Setiap window merupakan representasi dari pengirim untuk menunjukan total jumlah packet yang belum di acknowledge oleh penerima. Header dari packet menggunakan 16 bits untuk memuat besar window yang akan diterima kepada pengirim.

Pada awalnya, pengirim mulai untuk mengirim sedikit packet dan meningkatkan jumlah packet di setiap transmisi setelah menerima acknowledgement packet dari penerima. Setiap acknowledgement yang diterima, window bergeser satu packet untuk mentransmisi satu packet. 
Setiap satu ack diterima, window akan bergerak satu packet. Pada sisi penerima, window juga bergerak satu packet untuk setiap packet yang diterima. 

Paket yang dapat ditransmit pada Sliding Windows Protocol ini adalah 

```
na ≤ nr ≤ ns ≤ nt ≤ na + wt
```

dimana :
nt = sequence number pengirim saat ini
nr = sequence number penerima saat ini
wt = window size pengirim
nr = packet pertama yang belum diterima
ns = sequence number yang paling besar dari sequence number yang sudah diterima
na = acknowledgement terbesat yang diterima oleh pengirim

# Pembagian Tugas

13515081 Aldrich Valentino Halim :

13515084 Bethea Zia Davida :

13515120 Azis Adi Kuncoro :

# Jawaban Pertanyaan 

1.	Apa yang terjadi jika advertised window yang dikirim bernilai 0? Apa cara untuk menangani hal tersebut?

Ketika advertised window yang dikirim bernilai 0, client tidak dapat menerima data lagi dan transmisi dihentikan sampai bisa memproses informasi dalam buffer yang diterima.
Cara untuk menangani hal tersebut adalah :
1. Mengubah mode pengirim yang aktif untuk mengirim packet menjadi sleep untuk beberapa saat, lalu mencoba untuk mengirim packet lagi
2. Apabila penerima mendapat info advertised window yang bernilai 0, maka penerima akan melakukan pending pengiriman acknowledgment hingga advertised window bernilai lebih besar dari 0, setelah itu penerima baru mengirim ack berikutnya.

2.	Sebutkan field data yang terdapat TCP Header serta ukurannya, ilustrasikan, dan jelaskan kegunaan dari masing-masing field data tersebut!

TCP Header terdiri dari :
1.	Source Port Number (16 bits)
Kegunaan dari Source Port Number adalah untuk mengetahui port dari pengirim data.
2.	Destination Port Number (16 bits)
Kegunaan dari Destination Port Number adalah untuk mengetahui port dari penerima data.
3.	Sequence Number (32 bits)
Kegunaan dari Sequence Number adalah untuk menandai urutan dari data yang diterima, sehingga ketika nanti pecahan-pecahan data digabungkan, penerima tahu bagaimana urutannya.
4.	Acknowledgement Number (32 bits)
Kegunaan dari Acknowledgement Number adalah memberitahukan sequence number dari data yang baru saja dikirim ataupun diterima.
5.	Data Offset (4 bits)
Kegunaan dari Data Offset adalah untuk menyimpan ukuran dari TCP header dalam kelipatan 4 bytes.
6.	Reserved Data (3 bits)
Kegunaan dari Reserved Data adalah menyelaraskan total ukuran header menjadi kelipatan 4 bytes.
7.	ECN (Explicit Congestion Notification) (3 bits)
Kegunaan dari ECN adalah ekstensi control flags untuk mengatur data flow pada suatu kondisi yang spesifik.
8.	Control Bits (6 bits)
Kegunaan dari Control Bits sama dengan ECN yaitu mengatur data flow pada suatu kondisi yang spesifik
9.	Window Size (16 bits)
Kegunaan dari Windows Size adalah untuk meregulasi berapa banyak data yang dikirim kepada receiver sebelum meminta acknowledgement sebagai timbal baliknya.
10.	Checksum (16 bits)
Kegunaan dari Checksum adalah untuk mendeteksi adanya data yang error atau corrupt.
11.	Urgent Pointer (16 bits)
Urgent Pointer bisa digunakan sebagai Data Offset untuk menandai subset dari sebuah data sebagai suatu prioritas untuk diproses.
12.	Optional Data (0 to 40 bytes)
Kegunaan dari Optional Data adalah mendukung untuk acknowledgement khusus dan algoritma window scaling.

 -----------------------------------------------------------------------------------------
|	            	Source Port		                 |	          Destination Port	                |
|-----------------------------------------------------------------------------------------|
|		                              		Sequence Number		                                     	|
|-----------------------------------------------------------------------------------------|
|		                             	Acknoledgement Number        		        	                 |
|-----------------------------------------------------------------------------------------|
| Data Offset  | Reserved | ECN | Control Bits | 		        Window                        	|
|-----------------------------------------------------------------------------------------|
|		                  Checksum	    	         |	         Urgent Pointer                    	|
|-----------------------------------------------------------------------------------------|	
| 		                         Optional Data dan Padding	                            	     	|
|-----------------------------------------------------------------------------------------|
| 			                                    	Data				     	                                  |
 -----------------------------------------------------------------------------------------

