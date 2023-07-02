// eğer o page daha önce çağrıldı ve arrayde var ise HİT ARTAR
// YOKSA MİSS ARTAR VE ARRAYE EKLER
// yeni txt ye her geçişinde dispatch ++;
BIL461 HOMEWORK3 

Nasıl çalıştırılır ?
tasks.txt ve tasks.txt içinde yazan dosyalar önceden ayarlanır.
-> gcc 191101053_hw3.c -o hw3
-> ./hw3


Ödevin için boyutu dinamik olarak artan ve boyutu malloc ile alınıp realloc ile boyutu düzenlenen bir struct yapısı oluşturulur.

Ödevin ilk kısmında tasks.txt dosyasını okuyup veriler tutulur. 
Her satırın ilk elemanı ve text dosyalarının verileri tutulur. 
Her dosyanın boyutu hesaplanıp 4096 bölünerek page sayfaları hesaplanır. 

250 boyutlu içi -1 verisi ile doldurulmuş bir frame oluşturulur. Her dosya frame'de yer kalana kadar random verilerle doldurulur ve bastırılır.

Ödevin ikinci kısmında 5 boyutlu tlb arrayleri tutulur. Task valueleri için hit, miss, dispatch değerleri hesaplanır. 
Eğer tlb arrayinde daha önceden çağrılmış page varsa hit sayısı artar.
Eğer bulunmuyorsa miss sayısı artar ve tlbye ekler. 
Her yeni txt ye geçişinde dispatch sayısı artar. 