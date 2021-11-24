package utils.affichage;
import javax.swing.*;

import utils.couleurs.Couleur;
import utils.fileIo.ReadWritePoint;

import utils.vecteur.PointVisible;

import java.awt.*;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.io.IOException;

import java.util.ArrayList;

public class Vue extends JPanel implements MouseWheelListener, MouseListener, ActionListener{
	Color bgColor = Couleur.bg; // la couleur de fond de la fenêtre
	Color fgColor = Couleur.fg; // la couleur des lignes
	int width;
	ArrayList<PointVisible> points = new ArrayList<PointVisible>();

	// n : le nombre de lignes
	// width, height : largeur, hauteur de la fenêtre
	public Vue(int n, int width) {
		super();
		JButton b1 = new JButton("Enregistrer");
		b1.setActionCommand("b1");
		b1.addActionListener(this);
		JButton b2 = new JButton("Charger");
		b2.addActionListener(this);
		Box b = Box.createHorizontalBox();
		b.add(b1);b.add(Box.createHorizontalStrut(10));b.add(b2);
		add(b);
		setBackground(bgColor);
		this.width = width;
		setPreferredSize(new Dimension(width, width));
		System.out.println("initialisation avec n = "+n);
		initPoints(n, (width - 100)/2 , width/2, width/2);
		addMouseListener(this);
		addMouseWheelListener(this);
	}
	
	// initialisation random
	// NB: l'initialisation dans disque est à faire (exercice 1)
	
	public void initPoints(int n, int r, int x, int y){
			int xp,yp;
			points = new ArrayList<PointVisible>();
			for (int i = 0; i <n; i++){
				xp = random(0, width);
				yp = random(0, width);
				points.add(new PointVisible(xp,  yp));
				points.get(i).setLabel("Point "+i);
			}
	}
				
	// méthode utilitaire 
	// retourne un entier compris entre xmin et xmax
	int random(int xmin,int xmax){
		double dr = Math.random() * (double) (xmax - xmin) + (double) xmin;
		return (int) dr;
	}
		
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2d = (Graphics2D) g;
		g2d.setPaintMode(); 
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING,	RenderingHints.VALUE_ANTIALIAS_ON);	

		g2d.setColor(fgColor);
		
		for (PointVisible p: points) {
			p.dessine(g2d);
		}
	}

	@Override
	public void mouseClicked(MouseEvent e) {
		int n = points.size();
		if (e.getButton() == MouseEvent.BUTTON1){
			initPoints(n, width-50, width/2, width/2);
			repaint();
		}
	}

	@Override
	public void mouseEntered(MouseEvent arg0) {
	}

	@Override
	public void mouseExited(MouseEvent arg0) {
	}

	@Override
	public void mousePressed(MouseEvent arg0) {
	}

	@Override
	public void mouseReleased(MouseEvent arg0) {
	}
	
	@Override
	public void mouseWheelMoved(MouseWheelEvent e) {
	}
	@Override
	public void actionPerformed(ActionEvent e) {
		String testFile = "tmp.txt";
		ReadWritePoint rw = new ReadWritePoint(testFile);
	
		if(e.getActionCommand().equals("b1")){
			for (PointVisible s: points){
				rw.add(s.x+";" + s.y+";"+s.getLabel());
			}
			try {
				rw.write();
			} catch (IOException ex) {
				ex.printStackTrace();
			}
		}else{
			initFromFile(testFile);
			repaint();
		}
	}
	public void initFromFile(String f){
		ReadWritePoint rw = new ReadWritePoint(f);
		try {
			points = rw.read();
		} catch (IOException e) {
			e.printStackTrace();
		}
}

}
	

