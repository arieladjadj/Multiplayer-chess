using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace chessGraphics
{
    public partial class PieceSelection : Form
    {
        public PieceSelection()
        {
            InitializeComponent();
        }
        public pipe p;
        public bool color;
        public void initForm(bool white, pipe pipe)
        {
            this.p = pipe;
            this.color = white;
            if (white)
            {
                Queen.BackgroundImage = Properties.Resources.q_white;
                Rook.BackgroundImage = Properties.Resources.r_white;
                Bishop.BackgroundImage = Properties.Resources.b_white;
                Knight.BackgroundImage = Properties.Resources.n_white;
            }
            else
            {
                Queen.BackgroundImage = Properties.Resources.q_black;
                Rook.BackgroundImage = Properties.Resources.r_black;
                Bishop.BackgroundImage = Properties.Resources.b_black;
                Knight.BackgroundImage = Properties.Resources.n_black;
            }
            Queen.Click += chooseButton;
            Rook.Click += chooseButton;
            Bishop.Click += chooseButton;
            Knight.Click += chooseButton;

        }

        private void chooseButton(object sender, EventArgs e)
        {
            Button b = (Button)sender;
            switch (b.Name)
            {
                case "Queen":
                    if (color)
                    {
                        p.sendEngineMove("Q");
                    }
                    else
                    {
                        p.sendEngineMove("q");
                    }
                    break;
                case "Rook":
                    if (color)
                    {
                        p.sendEngineMove("R");
                    }
                    else
                    {
                        p.sendEngineMove("r");
                    }
                    break;
                case "Bishop":
                    if (color)
                    {
                        p.sendEngineMove("B");
                    }
                    else
                    {
                        p.sendEngineMove("b");
                    }
                    break;
                case "Knight":
                    if (color)
                    {
                        p.sendEngineMove("N");
                    }
                    else
                    {
                        p.sendEngineMove("n");
                    }
                    break;
            }
            this.Close();
        }
    }
}
