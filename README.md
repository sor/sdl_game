# sdl_game
SDL hello world game

Author: Jan "Sordid" Reitz

Neue Struktur
=

Vorgehen - TL;DR:
-
- von `GameState` Ableiten und `Events`, `Update`, `Render` implementieren
- seinen `GameState` in einer simplen von `Game` abgeleiteten Klasse instanzieren
- in `main()` seine `Game` Klasse mit `Run()` starten 

Dateien
-

_global.h_ - Diese Datei wird in jeder anderen included, sie enthält alle überall benötigten Includes und Aliase.

_gamebase.h_ - Hier befinden sich 2 Basisklassen die für das eigene Spiel abgeleitet werden
- `Game` - Basisklasse welche ein mal pro Spiel existiert  
- `GameState` - Basisklasse welche ein mal pro Spielabschnitt existiert 

_main.cpp_ - Hier ist das Beispielhaft von `Game` abgeleitete `MyGame` Klasse und die `main()` Funktion

_intro.h_ - Hier ist das Beispielhaft von `GameState` abgeleitete `IntroState` Klasse

_editor.h_ - Hier ist das Beispielhaft von `GameState` abgeleitete `EditorState` Klasse


Klassen
-

`Game`
-
- Basisklasse welche ein mal pro Spiel existiert
- Einstiegspunkt, führt den Gameloop auf dem aktuelllen `GameState` aus: Methode `Run()`
- Kümmert sich ums Öffnen des Fensters
- Kümmert sich um das Verwalten und Umschalten der `GameState`s: Methode `SetNextState(int)`
  
`GameState`
-
- Basisklasse welche ein mal pro Spielabschnitt existiert
- Enthält Methoden zum Initialisieren und Deinitialisieren (Laden und Entladen) des Spielabschnitts
- Implementiert die 3 wichtigen Methoden: `Events`, `Update`, `Render`
- Wird von Game verwaltet und aufgerufen

`MyGame`
-
- Beispiel Implementierung von `Game` in _main.cpp_
- Instanziert die 2 `GameState`s, startet mit dem `IntroState`

`IntroState`
-
- Beispiel Implementierung von `GameState` in _intro.h_
- Zeigt ein Bild und wartet auf beliebige Eingabe um zum nächsten State zu springen

`EditorState`
-
- Beispiel Implementierung von `GameState` in _editor.h_
- Zeigt eine Palette von Tiles oben links und eine verschiebbare Karte in die die Tiles platziert werden können
- Hier kann man sehen wie ein Kamera Offset verwendet wird
