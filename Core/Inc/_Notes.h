Eklenecek özellikler

Başlangıçta tristörler tekli sürülerek.
gerçek faz sırası bulunacak.

sıfır geçişi reenable için dinamik zamanlama yapılabilir.

birden fazla ölçüm yapılarak kontrol sistemine gönderilebilir.
şu an 200us de bir ölçüm yapılıyor. mesela 20us de bir ölçüm yapılır
ve ortalaması alınarak kontrol sistemine verilebilir.



n001
zc_start_delay_300u_arr_32 zero cross başlangıcına tim3 ile eklenen 300us gecikme sonucu
hesaplanan tim_arr_max yarı periyot sonuna denk gelmiyor. bu gecikmeden kaynaklı sağa kaymayı düzeltmek
için 32 bit timer da 300us ye karşılık gelen sayı hesaplanan timer trigger değerinden çıkarılıyor.

n002
bu thy_drv_en==1 burda olmak zorunda yoksa açılışta tristörler kısa süreliğine çalışıyor.

n003
r fazı yükseliş ile s fazı düşüş arasındaki süre farkı bölü periyot süresinin 6 da biri.
r fazı yükseliş ile t fazı düşüş arasındaki süre farkı bölü periyot süresinin 6 da biri.
faz sırasının durumuna göre tm_r_rise_tm_s_fall_1 ve tm_r_rise_tm_t_fall_1 den biri 1 diğeri 5 e yakın
ama tam integer olmayan sayılar oluyor.

n004
ikisini topla ve yuvarla. 6 olması lazım. bu stabil şebeke sinyali olduğunun işaretlerinden biri.

n005
tm_r_rise_tm_s_fall 6 da 1 periyot süresinin yüzde 10 yakınında mı.
öyle ise r fazı başlangıç olarak faz sırasına 1 veya rst diyelim.

n006
tm_r_rise_tm_s_fall_1 in 1 e yakın olan değişken olduğu durumda buna uygulanan bu formül ile yakın olduğu
tamsayıya yakınlığının yüzdesel değeri bulunuyor.
tm_r_rise_tm_t_fall_2 nin 5 yakın olduğu durum olduğu için 5 e bölünüyor. ondan sonra 1 den çıkarılarak devam
ediliyor.

n007
işte bu yakınlık yüzdesi de stabilite durumu ile ilgili bilgi veriyor.
burda şu an yüzde 10 yakınlık kabul ediliyor. bu değer değişebilir. inceleme yapılmalı.

n008
burada 40 yazıyorsa 40*50=2000 milisaniye stabilite göstergelerinin istendiği gibi kalması bekleniyor.

n009
systick interrupt'ı 20us
9.5ms/20us = 475 artım sonunda sıradaki exti interrupt'ı enable ediyor.
bir sonraki sıfır geçişi için.
// SysTick->LOAD  = 10999; 20us
//#define exti_int_endely_per 475

n010
bloke etmeden shift register sürme işlemi.

n011
batarya akımı yok
batarya inspection isteği var
cont sistem hedef voltaj minimum voltajdan fazla ise voltajı bir birim azalt
gerçek çıkış voltajı başlangıç voltajından 0.5v uzaklaşmışsa ve kontrol sistemi hedef voltajı ile gerçek voltaj birbirine yakın kalmışsa
kontrol sistemi çıkış voltajını baya bi değiştirmiş anlamına geliyor.
batarya bağlı iken, akım yokken kontrol sistemi çıkış votajını düşüremez. bu yüzden batarya bağlı değil.

n013
	herhangi bir anda akım oluşursa batarya var
	batarya voltajı  belli bir değerin altındaysa batarya tok

	voltaj tut
	aşağı doğru ilerle 0.1. en az min voltaja kadar inilebilir.


	gerçek voltaj hedef voltaja çok yakın, gerçek voltaj tutulan voltajdan 1 volt aşağı indi
	hedef voltaj tutulandan 1 volt aşağı geldi, gerçek voltaj ile tutulan arası voltaj farkı 0.6 dan fazla oldu
		batarya yok
	gerçek voltaj hedef voltajdan uzak, gerçek voltaj baslangic voltajdan 1 volt aşağı indirilemedi
	hedef voltaj baslangicdan 1 volt aşağı geldi ancak gerçek voltaj ile baslangic arası voltaj farkı 0.6 dan fazla olmadı.
		batarya var


	yukarı doğru ilerle en fazla baslangicdan 2 volt yukarı ve max float ı geçmeyecek kadar.
	bu iki limitten birine gelindiğinde veya gelinceye kadar
		akım oluştu batarya var.

	hedef 1 volt yukarı çekildi, gerçek ile başlangıç arası fazla, hedef ile gerçek arası da düşük (ok explanation)
		batarya bağlı değil


	max -> device max v
	basl+2		------
	cs hedef	------

	gerç 		------

	basl 		------

	gerç		------

	cs hedef	------
	min -> Batt_inspect_min

n014
/*======================================================================
 * handleButton()
 *
 * Çok-tuşlu projelerde tekrar kullanılan minik durum makinesi.
 * Bir kesme (örn. 10 µs timer) içinde her tuş için çağırılır.
 *
 * Davranış
 * --------
 * • İlk basışta hemen tetik (fireFlag = 1)
 * • Basılı kalırsa   500 ms  sonra   ikinci tetik
 * • Sonra basılı kaldıkça 100 ms’de bir tekrar tetik
 * • Bırakıldığında  50 ms  “kilit” (yeni basış yok) — debounce & repeat kırpma
 *
 * Parametreler
 * ------------
 * pinState        : GPIO seviyesi (1 = buton hâlen basılı, 0 = değil)
 *
 * isHeld          : (out) 1 ise “buton şuan basılı” durum bilgisi
 * releaseCnt      : (out) bırakıldıktan sonra geçen kesme sayısı
 * holdCnt         : (out) basılı kaldığı toplam kesme sayısı
 * nextRepeatEdge  : (out) sıradaki oto-tekrar eşiği
 * fireFlag        : (out) 1 olduğunda ana döngü gerçek işlevi çağırır
 *
 * Zaman Sabitleri (başka yerde makro)
 * -----------------------------------
 * FIRST_REPEAT_T      = 500 ms / kesme periyodu
 * NEXT_REPEAT_T       = 100 ms / kesme periyodu
 * RELEASE_DELAY_T     =  50 ms / kesme periyodu
 *
 * Notlar
 * ------
 * - Tüm sayaçlar volatile; ISR <-> main veri yarışı engellenir.
 * - Tuş bırakılınca isHeld sıfırlanır, releaseCnt yeniden başlar.
 * - lock bölümünde yalnızca releaseCnt++ yapılır, böylece kilit süresi
 *   dolana kadar yeni basış görülmez.
 *====================================================================*/
n015
Relay, röle, çıkış kontakları sistemi.
16 tane çıkış rölesi var.
bir tane sıralama vektörü var. adı REL_OUT_ORDER_vect[i]
hangi sırada hangi durumun rölesi var bunu kaydediyor
REL_OUT_ORDER_vect[0] START_STOP_REL
REL_OUT_ORDER_vect[1] VAC_LO_FC_REL
REL_OUT_ORDER_vect[2] LINE_FUSE_OFF_FC_REL
Kullanıcı sıralama değiştirince bu vektörde sağdaki elmanlar değişiyor.
Bu vektör sıkıştırılarak eeprom a kaydediliyor.
Açılışta eepromdan sıkıştırılmış data alınarak bu vektör oluşturuluyor.
generate_REL_OUT_ORDER_vect_from_eeprom_parts_fc();
Bir de bir tablo var. adı REL_OUT_TB[].
Açılışta bu tablo da güncelleniyor. REL_OUT_ORDER_vect_to_REL_OUT_TB();
Bu tablo, eepromdaki sıralamaya göre oluşturuluyor.
Bu tablo oluşturulurken gereken bazı bilgiler
Böylece elimizde kullanıcının isteğine göre sıralanmış röle durum tablosu oluşuyor.
Rölelerin durumlarındaki değişiklikler bu tabloya işleniyor.



























