program Project1;

{$APPTYPE CONSOLE}

uses
  SysUtils, Math;

const eps : extended = 0.0000005;

type vector2 = record
  x, y : extended;
end;

type line = record
  a, b, c : extended;
end;

var
  n, i, k : Integer;
  myPoint : vector2;
  verts : array[1..100001] of vector2;

function Equal(first, second : vector2) : boolean;
begin
  Equal := (first.x - second.x < eps) and (first.y - second.y < eps);
end;

Function RealEq(a, b:extended):Boolean; {?????? ?????}
begin
  RealEq := Abs(a-b)<= Eps
End; {RealEq}

Function RealMoreEq(a, b:extended):Boolean; {?????? ??? ?????}
begin
  RealMoreEq := a - b >= Eps
End; {RealMoreEq}

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

function Neg(v : vector2) : vector2;
var
  res : vector2;
begin
  res.x := -v.x;
  res.y := -v.y;
  Neg:= res;
end;

function VdotV(first, second : vector2) : extended;
begin
  VdotV:= first.x * second.x + first.y * second.y;
end;

function Distance(first, second : vector2) : extended;
begin
  Distance:= sqrt(sqr(first.x - second.x) + sqr(first.y - second.y));
end;

function CheckOnSegment(firstPoint, secondPoint, point : vector2) : boolean;
var
x, y, x1, y1, x2, y2 : extended;

begin
  CheckOnSegment := Distance(firstPoint, point) + Distance(secondPoint, point) -
    Distance(firstPoint, secondPoint) < eps;
    {x := point.x;
    y := point.y;
    x1 := firstPoint.x;
    x2 := secondPoint.x;
    y1 := firstPoint.y;
    y2 := secondPoint.y;
  CheckOnSegment := RealEq((x-x1)*(y2-y1)- (y-y1)*(x2-x1),0)and (((RealMoreEq(x,x1)and
    RealMoreEq( x2,x))Or (RealMoreEq(x,x2)and RealMoreEq( x1,x))))  }
  //CheckOnSegment := (VxV(VminusV(point, firstPoint), VminusV(secondPoint, point)) > -eps) and
    //(point.x - min(firstPoint.x, secondPoint.x) > -eps) and (max(firstPoint.x, secondPoint.x) - point.x > -eps) and
    //(point.y - min(firstPoint.y, secondPoint.y) > -eps) and (max(firstPoint.y, secondPoint.y) - point.y > -eps)

end;

function InsideAngle(firstRay, secondRay, pointRay : vector2) : boolean;
begin
  InsideAngle := ((VxV(firstRay, pointRay) * VxV(firstRay, secondRay) > -eps) and
             (VxV(secondRay, pointRay) * VxV(secondRay, firstRay) > -eps));
end;

function InsideTriangle(pointA, pointB, pointC, point : vector2) : boolean;
var
  sideAB, sideBC, sideAC : vector2;
begin
  sideAB := VminusV(pointB, pointA);
  sideBC := VminusV(pointC, pointB);
  sideAC := VminusV(pointC, pointA);
  InsideTriangle := InsideAngle(sideAB, sideAC, VminusV(point, pointA)) and
            InsideAngle(Neg(sideAB), sideBC, VminusV(point, pointB));
end;

function CheckBorder(vertex, leftPoint, rightPoint, point : vector2) : boolean;
var
  res : boolean;
begin
  res := CheckOnSegment(leftPoint, rightPoint, point);
  if (Equal(leftPoint, verts[2])) then
  begin
    res := res or CheckOnSegment(vertex, verts[2], point);
  end;
  if (Equal(rightPoint, verts[n])) then
  begin
    res := res or CheckOnSegment(vertex, verts[n], point);
  end;
  CheckBorder := res;
end;

procedure CheckPoint(point : vector2);
var
  leftRay, rightRay, pointRay, middleRay : vector2;
  left, right, middle : int64;
begin
  leftRay := VminusV(verts[2], verts[1]);
  rightRay := VminusV(verts[n], verts[1]);
  pointRay := VminusV(point, verts[1]);
  if (not InsideAngle(leftRay, rightRay, pointRay)) then
  begin
    WriteLn('OUTSIDE');
    Exit;
  end;

  left := 2;
  right := n;
  middle := 0;
  while (right - left > 1) do
  begin
    middle := left + (right - left) div 2;
    middleRay := VminusV(verts[middle], verts[1]);
    if (InsideAngle(leftRay, middleRay, pointRay)) then
    begin
      right := middle;
    end else
    begin
      left := middle;
    end;
  end;
  if (InsideTriangle(verts[1], verts[right], verts[left], point)) then
  begin
    if (CheckBorder(verts[1], verts[left], verts[right], point)) then
    begin
      WriteLn('BORDER');
    end else
    begin
      WriteLn('INSIDE');
    end;
  end else
  begin
    WriteLn('OUTSIDE');
  end;
end;

begin
  //reset(input, 'inside.in');
  //rewrite(output, 'inside.out');
  ReadLn(n);
  for i := 1 to n do
  begin
    ReadLn(verts[i].x, verts[i].y);
    {if (i > 3) then
    begin
      if (VxV(VminusV(verts[i - 2], verts[i - 3]), VminusV(verts[i - 1], verts[i - 2]))
       * VxV(VminusV(verts[i - 1], verts[i - 2]), VminusV(verts[i], verts[i - 1])) < eps) then
      begin
        WriteLn(verts[n+1000000].x);
        Halt;
      end;
    end;  }
  end;
  ReadLn(k);
  //k:=1;
  for i := 1 to k do
  begin
    ReadLn(myPoint.x, myPoint.y);
    CheckPoint(myPoint);
  end;
  //ReadLn;
  //close(output);
end.
