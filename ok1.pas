{ Teste 1: programa com if/else, operadores relacionais e comentario valido }
program Calculadora;
var
    x, y : integer;
    resultado : integer;
begin
    x := 15;
    y := 7;
    resultado := x + y;
    if resultado > 20 then
        resultado := resultado - 5
    else
        resultado := resultado + 10;
    resultado := resultado * 2;
    resultado := resultado / 4
end.
