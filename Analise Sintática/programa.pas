program Teste;
var
    x, y: integer;
    z: real;
begin
    x := 10;
    y := 20;
    z := x + y;
    if z > 25 then
        z := z - 5
    else
        z := z + 5;
    while x > 0 do
    begin
        x := x - 1;
    end;
end.
