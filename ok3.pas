{ Teste 3: programa completo com if aninhado, operadores relacionais variados e case-insensitive }
PROGRAM ExemploCompleto;
VAR
    a, b, c : INTEGER;
    media : REAL;
BEGIN
    a := 10;
    b := 20;
    c := 30;
    media := 3.14;
    if a <> b then
        a := a + 1;
    if b >= c then
        b := b - c
    else
        c := c * 2;
    if media <= 3.5 then
        a := a + b
end.
