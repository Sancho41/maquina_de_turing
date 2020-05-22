while True:
  try:
    
    cur_state, r1, r2, r3 = input().split(',')
    new_state, w1, w2, w3, d1, d2, d3 = input().split(',')
    print(f"{cur_state} {r1} {w1} {d1} : {r2} {w2} {d2} : {r3} {w3} {d3} {new_state}")

  except:
    break