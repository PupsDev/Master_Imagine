package utils.vecteur;

import java.awt.Color;
import java.awt.FontMetrics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.util.Comparator;

import utils.couleurs.Couleur;


public class PointVisible extends Rectangle implements Comparator<PointVisible>{
	public static int midWidth = 5;
	Color color = Couleur.nw;
	Role role ;
	private String label;
	Vecteur segment;
	
	public void setLabel(String label) {
		this.label = label;
	}

	public PointVisible(int x, int y, Role r) {
		super(x,y, 2 * PointVisible.midWidth,2 * PointVisible.midWidth);
		this.role = r;
		switch(role){
		case begin: 
			color = Couleur.beginPoint;
			break;
		case intersect: // blanc
			color = Couleur.intersection;
			break;
		case end: //rouge
			color = Couleur.endPoint;
			break;
		default:
			color = Couleur.fg;
	}
	}

	public PointVisible(int xp, int yp) {
		this(xp, yp, Role.undefined);
	}

	public void dessine(Graphics2D g2d) {
		g2d.setColor(color);
		g2d.fill(this);	
		drawLabel(g2d);
	}

	@Override
	public int compare(PointVisible p1, PointVisible p2) {
		return p2.x - p1.x;
	}	

	public void print() {
		System.out.println("x = " + x + " y = " + y+" w = " + width + " h = " + height + " role = "+role);
	}

	public void drawLabel(Graphics2D g) {
		FontMetrics fm = g.getFontMetrics();
		int centeredText = (int) (x - fm.stringWidth(getLabel())/2 + fm.stringWidth("_"));
		g.drawString(getLabel(), centeredText, (int) (y-2));
	}

	public String getLabel() {
		return label;
	}

}

