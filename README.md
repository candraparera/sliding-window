# TUGAS BESAR IF3130 JARINGAN KOMPUTER

## Petunjuk 
1. Lakukan kompilasi dengan pemanggilan perintah ‘make’ pada bash.
2. Run server terlebih dahulu dengan format ./recvfile <filename> <windowsize> <buffersize> <port>. 
3. Setelah bash untuk server berhasil dijalankan, buka bash baru untuk menjalankan client.
4. Jalankan program untuk client dengan format ./sendfile <filename> <windowsize> <buffersize> <destination_ip> <destination_port>.

## Cara Kerja Sliding Window
Setelah client mengirimkan data, server akan menerimanya dan memberikan respon berupa ACK untuk setiap paket yang telah diterimanya. Data ACK yang diterima oleh client merupakan sequence number untuk data berikutnya serta ukuran window saat ini. Kemudian client akan menggeser kepala windows ke data dengan sequence number sesuai pada ACK. Berikut merupakan potongan kode dari fungsi main pada sever.c yang mengatur algoritma sliding window pada program:

// slide windows with next_ack
shl_buffer(acked_message, window_size, next_ack);
shl_buffer(acked_status, window_size, next_ack);
last_acked += next_ack;

## Pembagian Tugas
Bonus: log file.
Pembacaan data file dari file system.
Operasi pengiriman data.
Operasi penerimaan data.
Penulisan data file ke file system.
Operasi pengiriman ACK.
Operasi penerimaan ACK.
Operasi penggeseran window.
Pembacaan buffer oleh aplikasi.
Penulisan buffer oleh aplikasi.
Candra Hesen Parera 13515019 -
Jauhar Arifin 13515049 -
Rizky Faramita 13515055 -

## Penanganan Advertised Window Bernilai 0
Data sewajarnya tidak akan diproses. Window size dengan lebar 0 mengindikasikan bahwa buffer penerima sudah penuh dan jika ada data yang masuk maka akan di-discard. Untuk mengatasi kasus tersebut, pengirim dapat mengirimkan segment kecil berisi satu byte payload secara berkala, jika penerima sudah memiliki kapasitas buffer untuk menampung segment tersebut, maka penerima akan mengirimkan ack dengan advertize window size yang lebih dari nol sehingga proses pengiriman data dapat berlangsung kembali.

## Field Data pada TCP Header
1. Source TCP Port Number (2 bytes): bertindak sebagai ujung dari sending device.
2. Destination TCP Port Number (2 bytes): bertindak sebagai ujung dari receiving device.
3. Sequence Number (4 bytes): bertindak sebagai penanda urutan dari pengiriman paket pesan.
4. Acknowledgement Number (4 bytes): bertindak sebagai penanda telah diterima atau belumnya suatu paket pesan. 
5. TCP Data Offset (4 bits): berguna untuk menyimpan ukuran total TCP Header ke dalam kelipatan 4 bytes.
6. Reserved Data (3 bits): berguna untuk memastikan bahwa ukuran total header selalu merupakan kelipatan dari 4 bytes. Oleh karena itu, reserved data selalu bernilai 0.
7. Control Flags (< 10 bits): bertindak sebagai penanda aliran data untuk setiap situasi yang terjadi. 
8. Window Size (2 bytes): berguna untuk membatasi seberapa banyak data yang akan dikirimkan oleh receiver sebelum menerima ACK dari server. Nilai window size yang terlalu kecil membuat proses transfer data menjadi sangat lama. Sebaliknya, kinerja receiver akan menurun apabila ukuran window size terlalu besar, karena sulit untuk memproses data yang baru datang dengan cepat.
9. TCP Checksum (2 bytes): berguna untuk membantu receiver mengenali pesan yang corrupted.
10. Urgent Pointer (2 bytes): berguna untuk pemberian level prioritas pada paket pesan.
11. TCP Optional Data (0-40 bytes): berguna untuk mengimplementasikan algoritma window scaling dalam penanganan ACK khusus. 


