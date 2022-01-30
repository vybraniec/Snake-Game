autor: Martyna Wybraniec

# Gra SNAKE: opis projektu

## ZASADY GRY:

Gra polega na sterowaniu "wężem" przy pomocy klawiszy strzałek. Po naciśnięciu klawisza "ENTER" w menu początkowym, gra się rozpoczyna i wąż zaczyna się poruszać. Gracz ma za zadanie kierować wężem w taki sposób, aby zjadł on jak najwięcej księżyców pojawiających się na planszy, jednocześnie nie dopuszczając do kolizji głowy węża z jego ogonem lub krawędzią planszy. Zjedzenie księżyca powoduje wydłużenie się węża o 1 człon. Gra nie ma limitu czasowego, kończy się jedynie wtedy, kiedy dojdzie do kolizji.

## OPIS PROGRAMU:

Program po wywołaniu wyświetla ekran startowy z komunikatem "PRESS ENTER TO PLAY". Naciśnięcie tego klawisza powoduje ustawienie pozycji początkowej węża na środek planszy, wywołanie funkcji wyświetlającej na planszy księżyc o losowych współrzędnych oraz rozpoczęcie animacji przesuwającej węża do przodu. 

Program po każdym ruchu przesunięciu głowy węża sprawdza, czy doszło do kolizji za pomocą funkcji check_collision(). Dodatkowo program sprawdza, czy współrzędne głowy węża równają się współrzędnym księżyca i otrzymując wartość true zwiększa długość węża, losuje nowe współrzędne owoca oraz dodaje punkt do ogólnej sumy punktów. 

Otrzymanie wartości true w funkcji check_collision() powoduje wyświetlenie komunikatu "GAME OVER" oraz "PRESS ENTER TO TRY AGAIN".

## DODATKOWE UWAGI:

Program używa biblioteki Simple DirectMedia Layer (SDL) wersja 2.0.20

Wszystkie elementy graficzne w programie stworzone zostały przez autora programu. Są one dostępne w folderze "IMAGES".
