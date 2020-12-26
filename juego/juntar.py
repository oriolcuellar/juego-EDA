



nombre="default"
nombre2=".res"

fichero_g=open("aa_grande.txt", "w")

for i in range (1000):
    num=str(i)
    texto=nombre+num+nombre2
    fichero_p=open(texto, "r")

    partida=fichero_p.readlines()[-4]
    fichero_g.write(partida)
    fichero_p.close()
fichero_g.close()




