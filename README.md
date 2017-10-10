# prepare_faces
Prepare Face Photos for Face Recognition

|kaynak_dizin
|-- Messi
|   |-- Messi1.jpg
|   |-- ...
|   |-- Messi10.jpg
|-- Robben
|   |-- Robben1.jpg
|   |-- ...
|   |-- Robben11.jpg
...
|-- Ibrahimovic
|   |-- Ibrahimovic1.jpg
|   |-- ...
|   |-- Ibrahimovic2.jpg

Örnek kullanım şekli:
./Prepare.exe /kaynak_dizin /hedef_bos_dizin

-> Kaynak dizindeki dosya ve klasör adları önemsiz.

-> Şimdilik sadece *.jpg dosyaları ile çalışıyor.

-> Klasör isimlerinden boşluk karakteri olmamalı.

-> Herhangi bir alt klasörde yüz tespiti yapılamadığı zaman hatalı çıktı üretiyor.
