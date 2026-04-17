
# GEORGE OTIENO NGIYE ,SCT211-0427/2023
# JEREMY MWIHIA ,SCT211-0028/2023

import pandas as pd
from OpenGL.GL import *
from OpenGL.GLUT import *

# storing the data in a pandas dataframe for easier manipulation
data = {
    'Day': ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'],
    'Earnings': [590, 850, 940, 1070, 800, 1020]
}
df = pd.DataFrame(data)

# scaling constants
minVal = df['Earnings'].min()
maxVal = df['Earnings'].max()
padding = 0.2


def scale_y(val):
    norm = (val - minVal) / (maxVal - minVal)
    return (-0.7) + norm * 1.4 

def get_x(i):
    return (-0.7) + (1.4) * i / (len(df) - 1)

# applying scaling to dataframe
df['x_coords'] = [get_x(i) for i in range(len(df))]
df['y_coords'] = df['Earnings'].apply(scale_y)

def draw_text(x, y, text):
    """Utility to render bitmap text at a specific coordinate"""
    glRasterPos2f(x, y)
    for char in text:
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ord(char))
def render():
    #cream background
    glClearColor(1.0, 0.99, 0.81, 1.0)
    glClear(GL_COLOR_BUFFER_BIT)
    
    #  Draw Axes 
    glColor3f(0.3, 0.3, 0.3) 
    glLineWidth(1.5)
    glBegin(GL_LINES)
    glVertex2f(-0.8, -0.8); glVertex2f(-0.8, 0.8) # Y-Axis
    glVertex2f(-0.8, -0.8); glVertex2f(0.8, -0.8) # X-Axis
    glEnd()

    #  Axis Titles 
    glColor3f(0, 0, 0) # Black text
    draw_text(-0.95, 0.85, "Earnings (Ksh)")
    draw_text(0.7, -0.9, "Days")

    #  Label Y-Axis Ticks (All Data Points) 
    # We loop through the DataFrame to label every actual earning value
    for i in range(len(df)):
        earning_label = str(df.iloc[i]['Earnings'])
        y_pos = df.iloc[i]['y_coords']
        # Position the text slightly to the left of the Y-axis (-0.98)
        draw_text(-0.98, y_pos, earning_label)

    #  Label X-Axis Ticks (Days) 
    for i in range(len(df)):
        day_label = df.iloc[i]['Day']
        x_pos = df.iloc[i]['x_coords']
        # Position the text slightly below the X-axis (-0.88)
        draw_text(x_pos - 0.05, -0.88, day_label)

    #  Draw Graph Line 
    glColor3f(0.0, 0.0, 1.0) # Blue Line
    glLineWidth(2.5)
    glBegin(GL_LINE_STRIP)
    for i in range(len(df)):
        glVertex2f(df.iloc[i]['x_coords'], df.iloc[i]['y_coords'])
    glEnd()

    #data points plotting 
    glColor3f(1.0, 0.0, 0.0) # Red Points
    glPointSize(10.0)
    glEnable(GL_POINT_SMOOTH)
    glBegin(GL_POINTS)
    for i in range(len(df)):
        glVertex2f(df.iloc[i]['x_coords'], df.iloc[i]['y_coords'])
    glEnd()
    
    glFlush()
def main():
    glutInit()
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)
    glutInitWindowSize(800, 600)
    glutCreateWindow(b"Earnings Graph")
    glutDisplayFunc(render)
    glutMainLoop()

if __name__ == "__main__":
    main()