program Project1;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Math;

type vector2 = record
  x, y : Integer;
end;

var
  n, i, firstEmpty, last, crossProduct, secondInd, firstInd : Integer;
  swapper, firstE, secondE, lastEdge, firstEdge, secondEdge, tmp, q : vector2;
  vertex : array[0..20000] of vector2;
  hull : array[0..20000] of Integer;

function Equal(first, second : vector2) : boolean;
begin
  Equal := (first.x = second.x) and (first.y = second.y);
end;

function GetLength(vector : vector2) : extended;
begin
  result := sqrt(vector.x * vector.x + vector.y * vector.y);
end;

function VxV(first, second : vector2) : Integer;
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

procedure Push(target : Integer);
begin
  hull[firstEmpty] := target;
  firstEmpty := firstEmpty + 1;
end;

function Peek : Integer;
begin
  result := hull[firstEmpty - 1];
end;

function Pop : vector2;
begin
  firstEmpty := firstEmpty - 1;
  result := vertex[hull[firstEmpty]];
end;

function Count : Integer;
begin
  result := firstEmpty;
end;

function Comparator(firstV, secondV : vector2) : Integer;
begin
  if (Equal(firstV, vertex[0])) then
  begin
    result := -1;
    Exit;
  end;
  if (Equal(secondV, vertex[0])) then
  begin
    result := 1;
    Exit;
  end;
  firstEdge := VminusV(firstV, vertex[0]);
  secondEdge := VminusV(secondV, vertex[0]);
  crossProduct := VxV(firstEdge, secondEdge);
  if (crossProduct <> 0) then
    result := -sign(crossProduct)
  else
    result := sign(GetLength(firstEdge) - GetLength(secondEdge));
end;

procedure QSort(l, r: Integer);
var i1, j1, t : Integer;
begin
  i1 := l;
  j1 := r;

  if (l >= r) then Exit;
  t := (l + r) div 2;
  q := vertex[t];
  repeat
    while (Comparator(vertex[i1], q) < 0) and (i1 <> t) and (i1 < n) do
    begin
      inc(i1);
    end;
    while (Comparator(q, vertex[j1]) < 0) and (j1 <> t) and (j1 >= 0) do
    begin
      dec(j1);
    end;
    if (i1 <= j1) then
    begin
      tmp := vertex[i1];
      vertex[i1] := vertex[j1];
      vertex[j1] := tmp;
      inc(i1);
      dec(j1);
    end;
  until i1 > j1;
  if (l < j1) then
    qSort(l, j1);
  if (i1 < r) then
    qSort(i1, r);
end;

begin
  reset(input, 'hull.in');
  rewrite(output, 'hull.out');
  ReadLn(n);

  for i := 0 to n - 1 do
  begin
    ReadLn(vertex[i].x, vertex[i].y);
    if ((vertex[i].x < vertex[0].x) or ((vertex[i].x = vertex[0].x) and
        (vertex[i].y < vertex[0].y))) then
    begin
      swapper := vertex[i];
      vertex[i] := vertex[0];
      vertex[0] := swapper;
    end;
  end;

  QSort(0, n - 1);
  Push(0);
  last := 1;

  while ((Equal(vertex[last], vertex[0])) and (last < n)) do
    inc(last);

  if (last = n) then
  begin
    WriteLn(1);
    WriteLn(vertex[0].x, ' ', vertex[0].y);
    Halt;
  end;

  firstE := vertex[0];
  secondE := vertex[last];

  while (VxV(VminusV(vertex[last + 1], secondE), VminusV(secondE, firstE)) = 0) do
  begin
    inc(last);
    if (last = n) then
    begin
      WriteLn(2);
      WriteLn(vertex[0].x, ' ', vertex[0].y);
      WriteLn(vertex[n - 1].x, ' ', vertex[n - 1].y);
      Halt;
    end;
    secondE := vertex[last];
  end;

  Push(last);
  firstE := vertex[0];
  secondE := vertex[last];

  secondInd := last;
  firstInd := 0;

  lastEdge := VminusV(secondE, firstE);
  for i := last + 1 to n - 1 do
  begin
    while (VxV(lastEdge, VminusV(vertex[i], secondE)) <= 0) do
    begin
      Pop;
      secondE := firstE;
      secondInd := firstInd;
      Pop;
      firstE := vertex[Peek];
      firstInd := Peek;
      push(secondInd);
      lastEdge := VminusV(secondE, firstE);
    end;
    Push(i);
    firstE := secondE;
    firstInd := secondInd;
    secondE := vertex[i];
    secondInd := i;
    lastEdge := VminusV(secondE, firstE);
  end;

  WriteLn(Count);
  if (n mod 2 = 0) then
    for i := 0 to Count - 1 do
      WriteLn(vertex[hull[i]].x, ' ', vertex[hull[i]].y)
  else
    for i := Count - 1 downto 0 do
      WriteLn(vertex[hull[i]].x, ' ', vertex[hull[i]].y);
  close(output);
end.
