program Project1;

{$APPTYPE CONSOLE}

uses
  SysUtils;

const eps = 0.000000001;

type vector2 = record
  x, y : extended;
end;

type line = record
  a, b, c : extended;
end;

var
  n, balance, i : Integer;
  myPoint, firstVert, lastVert, currentVert : vector2;

function Equal(first, second : vector2) : boolean;
begin
  Equal := (first.x - second.x < eps) and (first.y - second.y < eps);
end;

function Distance(first, second : vector2) : extended;
begin
  Distance:= sqrt(sqr(first.x - second.x) + sqr(first.y - second.y));
end;

function VxV(first, second : vector2) : extended;
begin
  VxV:= first.x * second.y - first.y * second.x;
end;

function VminusV(first, second : vector2) : vector2;
var
  res : vector2;
begin
  res.x := first.x - second.x;
  res.y := first.y - second.y;
  VminusV:= res;
end;

function VdotV(first, second : vector2) : extended;
begin
  VdotV:= first.x * second.x + first.y * second.y;
end;

function Neg(v : vector2) : vector2;
var
  res : vector2;
begin
  res.x := -v.x;
  res.y := -v.y;
  Neg:= res;
end;

function min(a, b : Real) : extended;
begin
  if (a < b) then
    min:= a
  else
    min:= b;
end;

function GetLine(first, second : vector2) : line;
var
  res : line;
begin
  res.a := second.y - first.y;
  res.b := first.x - second.x;
  res.c := (first.y - second.y) * first.x + (second.x - first.x) * first.y;
  GetLine := res;
end;

function CheckOnSegment(firstPoint, secondPoint, point : vector2) : boolean;
begin
  result := Distance(firstPoint, point) + Distance(secondPoint, point) -
    Distance(firstPoint, secondPoint) < eps;
end;

function IntersectRay(firstPoint, secondPoint, point : vector2) : boolean;
var
  segmentLine : line;
  intersectionPoint : vector2;
begin
  segmentLine := GetLine(firstPoint, secondPoint);
  if (segmentLine.a = 0) then
    result := false //сторона горизонтальна
  else
  begin
    intersectionPoint.x := (-segmentLine.c - segmentLine.b * point.y) / segmentLine.a;
    intersectionPoint.y := point.y;
    if (CheckOnSegment(firstPoint, secondPoint, intersectionPoint)) and
       (intersectionPoint.x - point.x > eps) then
    begin
      //если точка пересечения - конец отрезка, то игнорируем те, которые выше луча
      if (Equal(intersectionPoint, firstPoint)) then
        result := secondPoint.y < point.y - eps
      else
        if (Equal(intersectionPoint, secondPoint)) then
          result := firstPoint.y < point.y - eps
        else
          result := true;
    end else
      result := false;
  end;
end;

procedure CheckSegment(firstPoint, secondPoint, point : vector2);
begin
  if (CheckOnSegment(firstPoint, secondPoint, point)) then
    begin
      WriteLn('YES');
      close(output);
      Halt;
    end;
    if (IntersectRay(firstPoint, secondPoint, point)) then
    begin
      balance := balance + 1;
    end;
end;

begin
  //reset(input, 'belong.in');
  //rewrite(output, 'belong.out');
  ReadLn(n, myPoint.x, myPoint.y);
  ReadLn(firstVert.x, firstVert.y);
  lastVert := firstVert;
  balance := 0;
  for i := 2 to n do
  begin
    ReadLn(currentVert.x, currentVert.y);
    CheckSegment(lastVert, currentVert, myPoint);
    lastVert := currentVert;
  end;
  CheckSegment(firstVert, lastVert, myPoint);
  if (balance mod 2 <> 0) then
  begin
    WriteLn('YES');
  end
  else
  begin
    WriteLn('NO');
  end;
  close(output);
end.
