import math
import time

def ClosestPair(points):
    if len(points) <= 3:
        return getMinDistance(points)
    
    half = len(points)//2
    if len(points)%2 != 0:
        half += 1
    
    points_left = []
    points_right = []
    for i in range(len(points)):
        if(i < half):
            points_left.append(points[i])
        else:
            points_right.append(points[i])
    
    CP_L = ClosestPair(points_left);    dis_CP_L = getDistance(CP_L[0], CP_L[1])
    CP_R = ClosestPair(points_right);   dis_CP_R = getDistance(CP_R[0], CP_R[1])
    CP_C = getMidRange(points, dis_CP_L, dis_CP_R, half);   dis_CP_C = getDistance(CP_C[0], CP_C[1])

    min_dis = min(dis_CP_L, dis_CP_C, dis_CP_R)
    if min_dis == dis_CP_L:
        return CP_L
    elif min_dis == dis_CP_C:
        return CP_C
    else:
        return CP_R

    

def getMidRange(points, dis_CP_L, dis_CP_R, half):
    min = dis_CP_L
    if(dis_CP_R < min):
        min = dis_CP_R
    
    Mid_points = []
    for i in range(half, -1, -1):
        if points[half][0]-min <= points[i][0]:
            Mid_points.append(points[i])
        else:
            break

    for i in range(half+1, len(points), 1):
        if points[i][0] <= points[half+1][0]+min:
            Mid_points.append(points[i])
        else:
            break
    
    return getMinDistance(Mid_points)

def getMinDistance(points):
    min_points = []
    min = float('inf')

    for i in range(len(points)):
        for j in range(i+1, len(points)):
            point1 = points[i]
            point2 = points[j]
            distance = getDistance(point1, point2)

            if(distance == 0):
                pass
            elif len(min_points) == 0:
                min = distance
                min_points.append(point1)
                min_points.append(point2)
            elif distance < min:
                min = distance
                min_points[0] = point1
                min_points[1] = point2

    return min_points

def getDistance(point1, point2):
    x1, y1 = point1
    x2, y2 = point2
    return math.sqrt((x2-x1)**2 + (y2-y1)**2)

def main():
    points = []

    with open('input_closest_pair.txt', 'r') as file:
        for line in file:
            x, y = map(int, line.split())
            points.append((x, y))

    x_sorted_points = sorted(points, key=lambda point: point[0])
    
    Closest_Points = []
    start = time.time()
    Closest_Pair = ClosestPair(x_sorted_points)
    end = time.time()
    
    print(f"Closest Pair: {Closest_Pair} \tDistance: {getDistance(Closest_Pair[0], Closest_Pair[1])}")
    print(f"running time (s): {end-start}")

if __name__ == "__main__":
    main()
