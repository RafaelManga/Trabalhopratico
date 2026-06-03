{ Erro sintatico 3: EOF inesperado dentro de bloco }
program ErroSint3;
var
    x : integer;
begin
    x := 10;
    while x > 0 do
    begin
        x := x - 1;
    { falta end. e fim de arquivo chega no meio do bloco

