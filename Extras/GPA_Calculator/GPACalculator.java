import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.Component;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.Rectangle;
import java.text.DecimalFormat;
import javax.swing.JApplet;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SpringLayout;
import javax.swing.SwingUtilities;
import java.util.Scanner;

public class GPACalculator extends JApplet implements ActionListener {

//	private boolean resizeEvent1, resizeEvent2;
	private double x, y, z;
	private static int oldWidth1,oldWidth2,oldWidth3, gradeRows;
	private JButton[] button;
	private JComboBox[] comboBox;
	private JFrame frame;
	private JLabel label1, label2, label3;
	private JPanel[] p;
	private JTextField[] course;
	private JTextField finalGPA;
	private JTextField[] points;
	public Rectangle newBounds, oldBounds;
	private SpringLayout s1;
	private String[] description;

	public GPACalculator() {
		//resizeEvent1 = resizeEvent2 = false;
		button = new JButton[2];
		comboBox = new JComboBox[gradeRows + 2];
		p = new JPanel[gradeRows + 2];
		course = new JTextField[gradeRows];
		points = new JTextField[gradeRows];
		s1 = new SpringLayout();
		description = new String[] { "Select", "A+", "A", "A-", "B+", "B", "B-", "C+", "C", "C-", "D+", "D", "D-", "F" };
		p[0] = new JPanel(s1);
		for(int i=1; i<gradeRows+2; i++)
			p[i] = new JPanel(new FlowLayout());
		prepareGUI();
	}

	public void prepareGUI() {
		frame = new JFrame("GPA Calculator");
		//frame.setSize(400, 400);
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setLayout(new GridLayout(gradeRows+2,1));

		frame.setResizable(false);
	}

	public void init() {
		//Execute a job on the event-dispatching thread; creating this applet's GUI.
		label1 = new JLabel("<html><br>Course</html>");
		label2 = new JLabel("<html>Credit<br>Points</html>");
		label3 = new JLabel("<html><br>Grade</html>");
		p[0].add(label1);
		p[0].add(label2);
		p[0].add(label3);
		x = 95;
		y = 135;
		z = 72;
		s1.putConstraint(SpringLayout.NORTH, label1, 5, SpringLayout.NORTH, p[0]);
		s1.putConstraint(SpringLayout.NORTH, label2, 5, SpringLayout.NORTH, p[0]);
		s1.putConstraint(SpringLayout.NORTH, label3, 5, SpringLayout.NORTH, p[0]);
		s1.putConstraint(SpringLayout.WEST, label1, (int)x, SpringLayout.WEST, p[0]);
		s1.putConstraint(SpringLayout.WEST, label2, (int)y, SpringLayout.WEST, label1);
		s1.putConstraint(SpringLayout.WEST, label3, (int)z, SpringLayout.WEST, label2);
		frame.add(p[0]);
		
		for (int i = 0; i<gradeRows; i++) {
			course[i] = new JTextField(18);
			points[i] = new JTextField(6);
			p[i+1].add(course[i]);
			p[i+1].add(points[i]);
			comboBox[i] = new JComboBox<String>(description);
			p[i+1].add(comboBox[i]);
			frame.add(p[i+1]);
		}
		
		button[0] = new JButton("Clear All");
		button[1] = new JButton("Calculate your GPA");
		finalGPA = new JTextField(6);
		
		button[0].addActionListener(this);
		button[1].addActionListener(this);

		p[gradeRows + 1].add(button[0]);
		p[gradeRows + 1].add(button[1]);
		p[gradeRows + 1].add(finalGPA);
		frame.add(p[gradeRows + 1]);
		frame.pack();
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);
	}

	private String calculate() throws Exception {
		boolean check = false;
		double a1 = 0;
		double b1 = 0;
		double ans;
		DecimalFormat df = new DecimalFormat("#.00");
		try {
			for(int i=0; i<gradeRows; i++) {
				if(isNumeric(points[i].getText()) && !comboBox[i].getSelectedItem().equals("Select")) {
					double c1 = getGradeValue((String)comboBox[i].getSelectedItem());
					double d1 = Double.parseDouble(points[i].getText());
					if(d1>0)
						check = true;
					a1 = a1 + c1*d1;
					b1 = b1 + d1;
				}
			}
			if(check) {
				ans = a1/b1;
				return df.format(ans);
			}
			else 
				return "Err";
		}
		catch(Exception ex) {
			ex.printStackTrace();
			return "Err";
		}
	}

	public void actionPerformed(ActionEvent e) {

		try {
			if(e.getSource() == button[0]) {
				for(int i=0; i<course.length; i++) {
					course[i].setText("");
					points[i].setText("");
					comboBox[i].setSelectedIndex(0);
					finalGPA.setText("");
				}
			}
			else if(e.getSource() == button[1]) {
				finalGPA.setText(calculate());
			}
		}

		catch(Exception ex) {
			ex.printStackTrace();
		}
	}

	public static void main(String[] args) {
		Scanner input = new Scanner(System.in);
		System.out.print("Enter no. of subjects: ");
		gradeRows = input.nextInt();
		GPACalculator applet = new GPACalculator();
		applet.init();
	}
	
	public boolean isNumeric(String str) throws Exception {  
		try {  	double d = Double.parseDouble(str);  }  
		catch(NumberFormatException nfe) {  return false;  }  
		return true;
	}

	private double getGradeValue(String grade) {
		if(grade.equals("Select"))	return 0;
		else if(grade.equals("A+"))	return 4.33;
		else if(grade.equals("A"))	return 4.00;
		else if(grade.equals("A-"))	return 3.67;
		else if(grade.equals("B+"))	return 3.33;
		else if(grade.equals("B"))	return 3.00;
		else if(grade.equals("B-"))	return 2.67;
		else if(grade.equals("C+"))	return 2.33;
		else if(grade.equals("C"))	return 2.00;
		else if(grade.equals("C-"))	return 1.67;
		else if(grade.equals("D+"))	return 1.33;
		else if(grade.equals("D"))	return 1.00;
		else if(grade.equals("D-"))	return 0.67;
		else if(grade.equals("F"))	return 0;
		else 						return 0;
	}
}
