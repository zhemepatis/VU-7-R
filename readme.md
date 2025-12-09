# Temperatūros atvaizdavimas

## Užduotis
Sukurti sistemą, kuri matuotų temperatūrą ir jos reikšmę rodytų dviejuose 7 segmentų ekranuose. Be to, sistema turėtų perduoti surinktus duomenis per Wi-Fi ryšį į API, kuris išsaugo informaciją duomenų bazėje ir leidžia ją gauti pagal nurodytą laiko intervalą.

Taip pat sukurti vartotojo sąsają, kuri leistų pasirinkti laikotarpį, iš API gauti išsaugotus temperatūros duomenis ir juos pateikti grafike.

## Komponentai
Šiame darbe buvo panaudoti:
- ESP32
- 74HC595 šiftregistro grandynas
- 7-ių segmentų ekranas (5011AS) x 2
- n-tipo mosfetas (IRL Z44N) x 2
- Rezistoriai (1 kOhm) x 7
- Laidai

## Prototipo schema
Nuorodos į sukurto prototipo schemas:
- [Prototipo jungimas TinkerCAD aplinkoje](img/temperature_display_circuit.png) 
- Prototipo schemos: [I dalis](img/temperature_display_schema_01.png) [II dalis](img/temperature_display_schema_02.png)