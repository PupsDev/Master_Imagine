// Définition de la classe Triangle

class Spoke
{
    /** constructeur */
    constructor()
    {
        /** shader */

        let srcVertexShader = dedent
            `#version 300 es
            in vec2 glVertex;
            void main()
            {
                gl_Position = vec4(glVertex, 0.0, 1.0);
            }`;

        let srcFragmentShader = dedent
            `#version 300 es
            precision mediump float;
            out vec4 glFragColor;
            void main()
            {
                glFragColor = vec4(0.9, 0.9, 0.9, 1.0);
            }`;
        let srcFragmentShader2 = dedent
            `#version 300 es
            precision mediump float;
            out vec4 glFragColor;
            void main()
            {
                glFragColor = vec4(0.9, 0., 0.0, 1.0);
            }`;

        let srcFragmentShader3 = dedent
            `#version 300 es
            precision mediump float;
            out vec4 glFragColor;
            void main()
            {
                glFragColor = vec4(0.0, 0.9, 0.0, 1.0);
            }`;

        // compiler le shader de dessin
        this.m_ShaderId = Utils.makeShaderProgram(srcVertexShader, srcFragmentShader, "Spoke");
        this.m_ShaderId2 = Utils.makeShaderProgram(srcVertexShader, srcFragmentShader2, "Spoke2");
        this.m_ShaderId3 = Utils.makeShaderProgram(srcVertexShader, srcFragmentShader3, "Spoke2");

        // déterminer où sont les variables attribute
        this.m_VertexLoc = gl.getAttribLocation(this.m_ShaderId, "glVertex");
        this.m_VertexLoc = gl.getAttribLocation(this.m_ShaderId2, "glVertex");
        this.m_VertexLoc = gl.getAttribLocation(this.m_ShaderId3, "glVertex");

        /** VBOs */

        // créer et remplir le buffer des coordonnées
        
        this.vertices = [
        0.0, 0.0109,
        0.2174, 0.0109,
        0.2174, 0.0109,
        0.2826, 0.1196,
        0.2826, 0.1196,
        0.2994, 0.1104,
        0.2994, 0.1104,
        0.2391, 0.0109,
        0.2391, 0.0109,
        0.3261, 0.0109,
        0.3261, 0.0109,
        0.3696, 0.0761,
        0.3696, 0.0761,
        0.386, 0.0667,
        0.386, 0.0667,
        0.3478, 0.0109,
        0.3478, 0.0109,
        0.4783, 0.0109,
        0.4783,0.0109,
        0.5,0.,

        ];
        this.verticesM = [];
        this.verticesR = [];
        this.verticesN = [];

        


        this.verticesF = [];

        this.onMirrorC();
        this.onRotationC();

        for(let i = 0 ; i < this.vertices.length; i++)
        {
           this.verticesN.push(this.vertices[i]);

        }
         for(let i = 0 ; i < this.verticesM.length; i++)
        {
            this.verticesN.push(this.verticesM[i]);

        }
        for(let i = 0 ; i < this.verticesR.length; i++)
        {
           this.verticesN.push(this.verticesR[i]);

        }

        

        this.m_VertexBufferId = Utils.makeFloatVBO(this.vertices, gl.ARRAY_BUFFER, gl.STATIC_DRAW);
        this.m_VertexBufferId2 = Utils.makeFloatVBO(this.verticesM, gl.ARRAY_BUFFER, gl.STATIC_DRAW);
        this.m_VertexBufferId3 = Utils.makeFloatVBO(this.verticesR, gl.ARRAY_BUFFER, gl.STATIC_DRAW);

    }
    onMirrorC()
    {
       let matmat = mat2.create();
       matmat=mat2.fromValues( 1.,0.,
                                0.,-1.);
       let P=[];

        for(let i = 0 ; i < this.vertices.length; i+=2)
        {
            P.push( vec2.fromValues(this.vertices[i],this.vertices[i+1]))

        }
        for(let i = 0 ; i < P.length; i++)
        {            
            vec2.transformMat2(P[i], P[i], matmat);
        }

        for(let i = 0 ; i < P.length; i++)
        {

            this.verticesM.push(P[i][0]);
            this.verticesM.push(P[i][1]);

        }


    }
    onRotationC()
    {

        let P=[];

        for(let i = 0 ; i < this.vertices.length; i+=2)
        {
            P.push( vec2.fromValues(this.vertices[i],this.vertices[i+1]))

        }
         for(let i = 0 ; i < this.verticesM.length; i+=2)
        {
            P.push( vec2.fromValues(this.verticesM[i],this.verticesM[i+1]))

        }
        let theta = 60.*(Math.PI / 180);

        let matmat = mat2.create();
        matmat=mat2.fromValues( Math.cos(theta),-Math.sin(theta),
                                Math.sin(theta),Math.cos(theta));

        
        for(let j = 0 ; j < 5; j++)
        {    
            for(let i = 0 ; i < P.length; i++)
            {            
                vec2.transformMat2(P[i], P[i], matmat);
            }

             for(let i = 0 ; i < P.length; i++)
            {

                this.verticesR.push(P[i][0]);
                this.verticesR.push(P[i][1]);

            }
        }

    }
    onScale()
    {

        let P=[];
        for(let i = 0 ; i < this.verticesN.length; i+=2)
        {
            P.push( vec2.fromValues(this.verticesN[i],this.verticesN[i+1]))

        }


        let matmat = mat2.create();
        matmat=mat2.fromValues( 0.5,0.,
                                0.,0.5);

     
        for(let i = 0 ; i < P.length; i++)
        {            
            vec2.transformMat2(P[i], P[i], matmat);
        }

        for(let i = 0 ; i < P.length; i++)
        {

            this.verticesF.push(P[i][0]);
            this.verticesF.push(P[i][1]);
        }
        

    }
    onSheer()
    {

        let P=[];
        for(let i = 0 ; i < this.verticesN.length; i+=2)
        {
            P.push( vec2.fromValues(this.verticesN[i],this.verticesN[i+1]))

        }

        let theta = 15.*(Math.PI / 180);
        let matmat = mat2.create();
        matmat=mat2.fromValues( 1.,Math.tan(theta),
                                0.,1.);

     
        for(let i = 0 ; i < P.length; i++)
        {            
            vec2.transformMat2(P[i], P[i], matmat);
        }

        for(let i = 0 ; i < P.length; i++)
        {

            this.verticesN[2*i]=P[i][0];
            this.verticesN[2*i+1]=P[i][1];
        }
        

    }
    /** dessiner l'objet */
    onDraw()
    {
        // activer le shader
        gl.useProgram(this.m_ShaderId);

        // activer et lier le buffer contenant les coordonnées
        gl.bindBuffer(gl.ARRAY_BUFFER, this.m_VertexBufferId);
        gl.enableVertexAttribArray(this.m_VertexLoc);
        gl.vertexAttribPointer(this.m_VertexLoc, Utils.VEC2, gl.FLOAT, gl.FALSE, 0, 0);

        // dessiner un triangle avec les trois vertices
        gl.drawArrays(gl.LINES, 0, this.vertices.length);

        // désactiver le buffer
        gl.disableVertexAttribArray(this.m_VertexLoc);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        // désactiver le shader
        gl.useProgram(null);

       
       

    }
     onDrawM()
    {
         // activer le shader
        gl.useProgram(this.m_ShaderId2);


        // activer et lier le buffer contenant les coordonnées
        gl.bindBuffer(gl.ARRAY_BUFFER, this.m_VertexBufferId2);
        gl.enableVertexAttribArray(this.m_VertexLoc);
        gl.vertexAttribPointer(this.m_VertexLoc, Utils.VEC2, gl.FLOAT, gl.FALSE, 0, 0);


        // dessiner un triangle avec les trois vertices
        gl.drawArrays(gl.LINES, 0, this.verticesM.length);

        // désactiver le buffer
        gl.disableVertexAttribArray(this.m_VertexLoc);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        // désactiver le shader
        gl.useProgram(null);
    }
     onDrawR()
    {
                // activer le shader
        gl.useProgram(this.m_ShaderId3);
        // activer et lier le buffer contenant les coordonnées
        gl.bindBuffer(gl.ARRAY_BUFFER, this.m_VertexBufferId3);
        gl.enableVertexAttribArray(this.m_VertexLoc);
        gl.vertexAttribPointer(this.m_VertexLoc, Utils.VEC2, gl.FLOAT, gl.FALSE, 0, 0);


        // dessiner un triangle avec les trois vertices
        gl.drawArrays(gl.LINES, 0, this.verticesR.length);

        // désactiver le buffer
        gl.disableVertexAttribArray(this.m_VertexLoc);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        // désactiver le shader
        gl.useProgram(null);
    }
    onDrawF()
    {
        this.onScale();
        this.m_VertexBufferId4 = Utils.makeFloatVBO(this.verticesF, gl.ARRAY_BUFFER, gl.STATIC_DRAW);
                // activer le shader
        gl.useProgram(this.m_ShaderId);
        // activer et lier le buffer contenant les coordonnées
        gl.bindBuffer(gl.ARRAY_BUFFER, this.m_VertexBufferId4);
        gl.enableVertexAttribArray(this.m_VertexLoc);
        gl.vertexAttribPointer(this.m_VertexLoc, Utils.VEC2, gl.FLOAT, gl.FALSE, 0, 0);


        // dessiner un triangle avec les trois vertices
        gl.drawArrays(gl.LINES, 0, this.verticesF.length);

        // désactiver le buffer
        gl.disableVertexAttribArray(this.m_VertexLoc);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        // désactiver le shader
        gl.useProgram(null);
    }
    onDrawFS()
    {
        this.onSheer();
        this.m_VertexBufferId4 = Utils.makeFloatVBO(this.verticesN, gl.ARRAY_BUFFER, gl.STATIC_DRAW);
                // activer le shader
        gl.useProgram(this.m_ShaderId);
        // activer et lier le buffer contenant les coordonnées
        gl.bindBuffer(gl.ARRAY_BUFFER, this.m_VertexBufferId4);
        gl.enableVertexAttribArray(this.m_VertexLoc);
        gl.vertexAttribPointer(this.m_VertexLoc, Utils.VEC2, gl.FLOAT, gl.FALSE, 0, 0);


        // dessiner un triangle avec les trois vertices
        gl.drawArrays(gl.LINES, 0, this.verticesF.length);

        // désactiver le buffer
        gl.disableVertexAttribArray(this.m_VertexLoc);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);

        // désactiver le shader
        gl.useProgram(null);
    }

    /** destructeur */
    destroy()
    {
        // supprimer le shader et le VBO
        Utils.deleteShaderProgram(this.m_ShaderId);
        Utils.deleteVBO(this.m_VertexBufferId);
    }
}
