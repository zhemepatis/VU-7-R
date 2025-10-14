# Žaidimas "Pong"

## Užduotis
Sukurti sistemą aptinkančią pavojingą  dujų lygmenį ir paleidžiančią įspėjamąjį signalą bei įjungiantį specialaus atsako sistemą. Šiuo atveju specialus atsako sistema yra ventiliacija. 

## Komponentai
Šiame darbe buvo panaudoti:
- Arduino UNO R3
- Potenciometrai (10 kOhm) x 2
- MAX7219 modulis
- Mygtukas x 2
- Laidai

## Prototipo schema
Nuorodos į sukurto prototipo schemas:
- [Prototipo jungimas TinkerCAD aplinkoje](img/pong_game_circuit.png) 
- [Prototipo schema](img/pong_game_schema.png)

## Tolimesni patobulinimai

### Laidų jungtys
Kaip galima buvo pastebėti vaizdo įraše, vienas iš potenciometrų grąžina nestabilius duomenis, net jeigu ir nėra keičiama jo padėtis. Tai trikdo žaidimo eigą ir vienam iš žaidėjų suteikia pranašumą. Šios problemos sprendimas būtų užtiktinti geresnį laidų kontaktą.

### Žaidimo sudėtingumo nustatytmas
Šiuo metu žaidimo eiga yra labai monotoniška, žaidėjams nėra sukuriamas iššūkis, todėl galima būtų pridėti lygio didinimą kas tam tikrą kamuoliuko atmušimo kiekį arba rankinį lygio pasirinkimo nustatymą, saugomą atmintyje.

### Žaidimo patirties tobulinimas
Dabartinėje žaidimo versijoje nėra skaičiuojami taškai, nėra aiškiai nurodoma, kam yra skiriamas taškas, kas laimėjo ir pan. Šios srities patobulinimas padarytų žaidimo patirtį išbaigtesnę ir leistų labiau mėgautis žaidimu.

### Didesnė matrica
Galima būtų prijungti dar kelias 8x8 matricas, kad žaidimo langas būtų didesnis ir žaidimo