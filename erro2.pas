{ Teste de erro 2: multiplos caracteres invalidos $ e % }
program ErroMultiplo;
var
    y : real;
begin
    y := 3.14;
    y := y $ 2;
    y := y % 3
end.
