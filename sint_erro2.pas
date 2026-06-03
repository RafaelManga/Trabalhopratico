{ Erro sintatico 2: if sem then }
program ErroSint2;
var
    x : integer;
begin
    x := 10;
    if x > 0
        x := x - 1;
end.

