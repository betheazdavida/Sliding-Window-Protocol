# Petunjuk Penggunaan Program

Penggunaan program dimulai dengan menjalankan comand :

```
./sendfile <filename> <windowsize> <buffersize> <destination_ip> <destination_port>
```

pada terminal yang ingin digunakan sebagai pengirim. Keterangan:
1. Tag filename diisi dengan nama file yang akan dikirim,
2. Tag windowsize diisi dengan windowsize yang diinginkan,
3. Tag buffersize diisi dengan besar buffer yang ingin dikirim,
4. Tag destination_ip diisi dengan ip dari penerima file, dan
5. Tag destination_port diisi dengan port yang digunakan oleh penerima untuk membangun koneksi dengan pengirim.

Setelah itu, membuka terminal lain yang ingin digunakan sebagai penerima dan menjalankan command

```
./recvfile <filename> <windowsize> <buffersize> <port>
```

Keterangan:
1. Tag filename diisi dengan nama file yang akan dikirim,
2. Tag windowsize diisi dengan windowsize yang diinginkan,
3. Tag buffersize diisi dengan besar buffer yang ingin dikirim, dan
4. Tag destination_port diisi dengan port yang digunakan oleh penerima untuk membangun koneksi dengan pengirim.


# Cara Kerja Sliding Window Protocol

Sliding window protocol adalah salah suatu protokol *control flow* untuk melakukan transmisi data berbasis packet dan menjamin transmisi yang reliable. Secara konsep, setiap transmisi yang dilakukan dilengkapi dengan *consecutive sequence number* yang unik. Penerima akan menggunakan *sequence number* tersebut untuk mengurutkan paket yang diterima dan menolak paket yang duplikat serta mengidentifikasi paket yang hilang. Setelah suatu paket diterima oleh penerima, maka penerima akan mengembalikan suatu *acknowledgement* kepada pengirim. Tujuan dari pengiriman *acknowledgement* adalah memberi tahu pengirim bahwa paket sudah diterima dan penerima siap untuk menerima paket berikutnya. 

Karakteristik dari protokol ini adalah paket dapat dikirimkan lebih dari satu. Jumlah paket yang dapat dikirim dalam satu kali pengiriman dianalogikan sebagai `window size`. Paket-paket yang berada dalam suatu window akan dikirimkan secara bersamaan. Paket-paket dalam window memiliki arti bahwa mereka `sudah dikirim tetapi belum menerima ACK`. Paket-paket yang berada sebelum window menandakan bahwa paket tersebut `sudah terkirim dan sudah ACK`. Paket-paket setelah window `belum dikirim dan belum ACK`. Ketika suatu paket menerima ACK, maka paket tersebut akan dikeluarkan dari window. Fenomena tersebut yang disebut sebagai *sliding window*, yaitu window seakan-akan bergeser dan menyesuaikan paket mana yang akan dikirim selanjutnya.

![alt text](https://github.com/betheazdavida/Sliding-Window-Protocol/blob/master/data/slidingwindow.jpg "Sliding Window")

Sliding window protocol membuat kita dapat melakukan transfer packet dengan sequence number yang besarnya sudah ditentukan dengan tidak terbatas. Header dari packet menggunakan 16 bits untuk memuat besar window yang akan diterima kepada pengirim. Jika terdapat paket yang hilang atau rusak, maka sender tidak akan mengirimkan ACK untuk paket tersebut. Hal ini mengimplikasikan bahwa window pada pengirim tidak akan bergeser sehingga setelah beberapa waktu tertentu, pengirim akan melakukan pengiriman ulang terhadap paket yang sama.

Cara kerja sliding window pada program:
1. Pengirim dan penerima melakukan koneksi ke socket UDP.
2. Pengirim membaca file dan memasukannya ke dalam file buffer (menggunakan fungsi `readToByte()`).
3. Pengirim mulai melakukan copy sedikit demi sedikit data dari file buffer ke dalam buffer. Pemindahan data sebesar data yang akan dikirim. Terdapat variabel `currentSeqnum` yang berfungsi sebagai penunjuk mulainya window pada buffer. Pada awal pengiriman, currentSeqNum diinisiasi dengan 0.
4. Pengirim mengirimkan pesan dengan fungsi `sendto()`. 
5. Pada sisi penerima, penerima mendengar (listen) pada port yang sudah didefinisikan dengan menggunakan `while(1)`. Pengirim akan menerima sejumlah paket. 
6. Paket akan dicek headernya, bila headernya sama dengan -1, maka pengiriman file sudah selesai dan proses mendengar selesai.
6. Paket-paket yang diterima akan dicek akan error dengan fungsi `isCheckSumEqual()`.
7. Jika paket memiliki checksum yang valid, maka data dalam paket akan dicopy ke dalam buffer penerima. Setelah dicopy ke dalam buffer, maka penerima akan membuat paket ACK terhadap paket tersebut. NextSequenceNumber yang diisi pada ACK adalah sequence number pada paket yang diterima ditambah 1. AWS akan diisi dengan selisih dari buffersize dengan currentSeqNum dari window penerima. Jika hasil selisih lebih besar dari window size, maka AWS diisi dengan window size. Jika tidak, maka akan diisi dengan selisih tersebut.
8. Di sisi pengirim, setelah pengirim selesai mengirim semua paket, maka pengirim akan mendengarkan (listen) untuk menerima ACK. Setiap ACK yang diterima akan dicek apakah checksumnya valid. Jika checksum valid, maka akan dilihat nextSequenceNumber. Setelah itu, currentSeqnum akan di-update dengan nextSequenceNumber.
9. Proses diulang dari langkah ketiga. Ketika proses pengiriman selesai, program pengirim akan mengirimkan suatu paket dengan header -1. Jika buffer pada penerima penuh, maka akan dilakukan proses write dari buffer ke file buffer penerima, lalu buffer dikosongkan.
10. Setelah proses pengiriman paket selesai, maka penerima akan melakukan proses write ke file yang dilakukan dengan fungsi `writeToFile()`.

# Pembagian Tugas

13515081 Aldrich Valentino Halim : koneksi socket UDP, debugging makefile, fungsi sendfile, fungsi recvfrom, debugging sliding window, readme

13515084 Bethea Zia Davida : checksum, struktur data paket, struktur data ack, readme, debugging sliding window

13515120 Azis Adi Kuncoro : readToByte, writeToFile, debugging koneksi socket UDP, pembuatan makefile, fungsi sendfile, fungsi recvfrom, debugging sliding window 

# Jawaban Pertanyaan 

## 1.	Apa yang terjadi jika advertised window yang dikirim bernilai 0? Apa cara untuk menangani hal tersebut?

Ketika advertised window yang dikirim bernilai 0, client tidak dapat menerima data lagi dan transmisi dihentikan sampai bisa memproses informasi dalam buffer yang diterima.
Cara untuk menangani hal tersebut adalah :
 1. Mengubah mode pengirim yang aktif untuk mengirim packet menjadi sleep untuk beberapa saat, lalu mencoba untuk mengirim packet lagi
 2. Apabila penerima mendapat info advertised window yang bernilai 0, maka penerima akan melakukan pending pengiriman acknowledgment hingga advertised window bernilai lebih besar dari 0, setelah itu penerima baru mengirim ack berikutnya.

## 2.	Sebutkan field data yang terdapat TCP Header serta ukurannya, ilustrasikan, dan jelaskan kegunaan dari masing-masing field data tersebut!

TCP Header terdiri dari:
### 1. Source Port Number (16 bits)
Kegunaan dari Source Port Number adalah untuk mengetahui port dari pengirim data. 
### 2. Destination Port Number (16 bits)
Kegunaan dari Destination Port Number adalah untuk mengetahui port dari penerima data.
### 3. Sequence Number (32 bits)
Kegunaan dari Sequence Number adalah untuk menandai urutan dari data yang diterima, sehingga ketika nanti pecahan-pecahan data digabungkan, penerima tahu bagaimana urutannya. 
### 4. Acknowledgement Number (32 bits)
Kegunaan dari Acknowledgement Number adalah memberitahukan sequence number dari data yang baru saja dikirim ataupun diterima.
### 5. Data Offset (4 bits)
Kegunaan dari Data Offset adalah untuk menyimpan ukuran dari TCP header dalam kelipatan 4 bytes.
### 6. Reserved Data (3 bits)
Kegunaan dari Reserved Data adalah menyelaraskan total ukuran header menjadi kelipatan 4 bytes.
### 7. ECN (Explicit Congestion Notification) (3 bits)
Kegunaan dari ECN adalah ekstensi control flags untuk mengatur data flow pada suatu kondisi yang spesifik.
### 8. Control Bits (6 bits)
Kegunaan dari Control Bits sama dengan ECN yaitu mengatur data flow pada suatu kondisi yang spesifik
### 9. Window Size (16 bits)
Kegunaan dari Windows Size adalah untuk meregulasi berapa banyak data yang dikirim kepada receiver sebelum meminta acknowledgement sebagai timbal baliknya.
### 10. Checksum (16 bits)
Kegunaan dari Checksum adalah untuk mendeteksi adanya data yang error atau corrupt.
### 11. Urgent Pointer (16 bits)
Urgent Pointer bisa digunakan sebagai Data Offset untuk menandai subset dari sebuah data sebagai suatu prioritas untuk diproses.
### 12. Optional Data (0 to 40 bytes)
Kegunaan dari Optional Data adalah mendukung untuk acknowledgement khusus dan algoritma window scaling.

![alt text](https://github.com/betheazdavida/Sliding-Window-Protocol/blob/master/data/tcpheader.svg "Ilustrasi TCP Header")
