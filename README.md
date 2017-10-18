#Petunjuk Penggunaan Program

Penggunaan program dimulai dengan menjalankan comand :
./sendfile <filename> <windowsize> <buffersize> <destination_ip> <destination_port>
pada terminal yang ingin digunakan sebagai pengirim. Pada :
1.	<filename> diisi dengan nama file yang akan dikirim
2.	<windowsize> diisi dengan windowsize yang diinginkan
3.	<buffersize> diisi dengan besar buffer yang ingin dikirim
4.	<destination_ip> diisi dengan ip dari penerima file
5.	<destination_port> diisi dengan port yang digunakan oleh penerima untuk membangun koneksi dengan pengirim.
Setelah itu, membuka terminal lain yang ingin digunakan sebagai penerima dan menjalankan command :
./recvfile <filename> <windowsize> <buffersize> <port>
Pada :
1.	<filename> diisi dengan nama file yang ingin digunakan untuk menerima file dari pengirim
2.	<windowsize> diisi dengan windowsize yang diinginkan
3.	<buffersize> diisi dengan besar buffer yang ingin dikirim
4.	<port> diisi dengan port yang ingin digunakan untuk menerima file.

#Cara Kerja Sliding Windows

#Pembagian Tugas

13515081 Aldrich Valentino Halim :
13515084 Bethea Zia Davida :
13515120 Azis Adi Kuncoro :

#Jawaban Pertanyaan 

1.	Apa yang terjadi jika advertised window yang dikirim bernilai 0? Apa cara untuk menangani hal tersebut?

Ketika advertised window yang dikirim bernilai 0, client tidak dapat menerima data lagi dan transmisi dihentikan sampai bisa memproses informasi dalam buffer yang diterima.
Cara untuk menangani hal tersebut adalah dengan melakukan pencarian penyebab mengapa terjadi hal tersebut, kemungkinan-kemungkinannya adalah mesin yang digunakan menjalankan banyak proses pada saat tersebut, sehingga prosesornya bekerja maksimal ataupun ada pembatasan dalam penggunaan RAM untuk transmisi tersebut. Selain itu bisa juga terjadi error pada penerima TCP seperti salah mengkonfigurasi Windows registry. Setelah diketahui penyebabnya, maka ditangani sesuai penyebabnya, contoh untuk salah mengkonfigurasi Windows registry, kita dapat melakukan pengubahan pada Windows registry nya.

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
 ------------------------------------------------------------------------------------------
|	            	Source Port		                 |	          Destination Port	            |
|-----------------------------------------------------------------------------------------|
|		                              		Sequence Number		                                 	|
|-----------------------------------------------------------------------------------------|
|		                             	Acknoledgement Number        		        	              |
|-----------------------------------------------------------------------------------------|
| Data Offset  | Reserved | ECN | Control Bits | 		        Window                      	|
|-----------------------------------------------------------------------------------------|
|		                  Checksum	    	         |	         Urgent Pointer                	|
|-----------------------------------------------------------------------------------------|	
| 		                         Optional Data dan Padding	                            		|
|-----------------------------------------------------------------------------------------|
| 			                                    	Data				     	                          |
 -----------------------------------------------------------------------------------------

