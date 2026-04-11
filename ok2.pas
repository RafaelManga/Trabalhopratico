{ Teste 2: programa com while, numero real e multiplas variaveis }
program ContadorReal;
var
    contador : integer;
    soma : real;
    limite : integer;
begin
    contador := 0;
    soma := 0.0;
    limite := 5;
    while contador < limite do
    begin
        soma := soma + 1.5;
        contador := contador + 1
    end
end.
