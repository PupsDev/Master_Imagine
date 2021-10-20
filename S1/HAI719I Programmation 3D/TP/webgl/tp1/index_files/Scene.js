// Définition de la classe Scene

// superclasses et classes nécessaires
Requires("Spoke");


class Scene
{
    /** constructeur */
    constructor()
    {
        // créer les objets à dessiner
        this.m_spoke = new Spoke();
        

        // couleur du fond : gris foncé
        gl.clearColor(0.4, 0.4, 0.4, 0.0);
    }


    /**
     * appelée quand la taille de la vue OpenGL change
     * @param width : largeur en nombre de pixels de la fenêtre
     * @param height : hauteur en nombre de pixels de la fenêtre
     */
    onSurfaceChanged(width, height)
    {
        // met en place le viewport
        gl.viewport(0, 0, width, height);
    }


    /**
     * Dessine l'image courante
     */
    onDrawFrame()
    {
        const {width, height} = gl.canvas;
        const leftWidth = width / 3 | 0;
        const heightd = height/2;

        gl.viewport(0, heightd, leftWidth, heightd);

        // effacer l'écran
        gl.clear(gl.COLOR_BUFFER_BIT);
        
        // dessiner le triangle
        this.m_spoke.onDraw();

        gl.viewport(leftWidth, heightd, leftWidth, heightd);


        this.m_spoke.onDrawM();

        gl.viewport(2*leftWidth, heightd, leftWidth, heightd);

        this.m_spoke.onDrawR();

        gl.viewport(0, 0, leftWidth, heightd);
        this.m_spoke.onDraw();
        this.m_spoke.onDrawM();
        this.m_spoke.onDrawR();

        gl.viewport(leftWidth, 0, leftWidth, heightd);
        this.m_spoke.onDrawF();

        gl.viewport(2*leftWidth, 0, leftWidth, heightd);
        this.m_spoke.onDrawFS();

    }


/** supprime tous les objets de cette scène */
    destroy()
    {
        this.m_spoke.destroy();
    }
}
