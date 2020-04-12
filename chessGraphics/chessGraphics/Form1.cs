using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;


namespace chessGraphics
{



    public partial class Form1 : Form
    {
        private readonly object pipeLock = new object();
        private Square srcSquare;
        private Square dstSquare;

        private pipe enginePipe;
        private pipe changePipe;
        private pipe chatPipe;
        Button[,] matBoard;

        bool isCurPlWhite = true;
        bool isGameOver = false;

        const int BOARD_SIZE = 8;

        public Form1()
        {
            InitializeComponent();
        }

        
        private void initForm()
        {
            enginePipe.connect();
            changePipe.connect();
            chatPipe.connect();
            Invoke((MethodInvoker)delegate
            {
                wait.Visible = true;
                lblWaiting.Visible = false;
            });
            enginePipe.getEngineMessage();
            Invoke((MethodInvoker)delegate {

                wait.Visible = false;
                lblCurrentPlayer.Visible = true;
                label1.Visible = true;
                chatBox.SelectionStart = chatBox.Text.Length;
                chatBox.ScrollToCaret();
                this.msgBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(CheckEnterKeyPress);

                string s = enginePipe.getEngineMessage();

                if (s.Length != (BOARD_SIZE * BOARD_SIZE + 1))
                {
                    MessageBox.Show("The length of the board's string is not according the PROTOCOL");
                    this.Close();

                }
                else
                {
                    isCurPlWhite = (s[s.Length - 1] == '0');
                    paintBoard(s);
                }

            });
    
        }

        Thread connectionThread;
        Thread changePipeThread;
        Thread chatPipeThread;
        private void Form1_Load(object sender, EventArgs e)
        {
            enginePipe = new pipe("chessPipe");
            changePipe = new pipe("changePipe");
            chatPipe = new pipe("chatPipe");
            //this.Show();
            
            //MessageBox.Show("Press OK to start waiting for engine to connect...");
            connectionThread = new Thread(initForm);
            connectionThread.Start();
            connectionThread.IsBackground = true;
            changePipeThread = new Thread(changePipeHandler);
            changePipeThread.Start();
            changePipeThread.IsBackground = true;

            //initForm();
        
        }

        Image getImageBySign(char sign)
        {
            switch (sign)
            {
                case 'q':
                    return Properties.Resources.q_black;
                case 'Q':
                    return Properties.Resources.q_white;
                case 'k':
                    return Properties.Resources.k_black;
                case 'K':
                    return Properties.Resources.k_white;
                case 'p':
                    return Properties.Resources.p_black;
                case 'P':
                    return Properties.Resources.p_white;
                case 'r':
                    return Properties.Resources.r_black;
                case 'R':
                    return Properties.Resources.r_white;
                case 'n':
                    return Properties.Resources.n_black;
                case 'N':
                    return Properties.Resources.n_white;
                case 'b':
                    return Properties.Resources.b_black;
                case 'B':
                    return Properties.Resources.b_white;
                case '#':
                    return null;
                default:
                    return Properties.Resources.x;

            }
        }

        private void paintBoard(string board)
        {
            int i, j, z=0;

            matBoard = new Button[BOARD_SIZE, BOARD_SIZE];

            btnBoard.FlatAppearance.MouseOverBackColor = Color.LightGray;
            
            Button newBtn;
            Point pnt;

            int currentWidth = btnBoard.Location.X;
            int currentHeight = btnBoard.Location.Y;
            
            bool isColBlack = true;
            bool isRowBlack = true ;
            
            this.SuspendLayout();

            lblCurrentPlayer.Text = isCurPlWhite ? "White" : "Black";

            for (i = 0; i < BOARD_SIZE; i++)
            {
                currentWidth = btnBoard.Location.X;
                isColBlack = isRowBlack;

                for (j = 0; j < BOARD_SIZE; j++)
                {
                    newBtn = new Button();
                    matBoard[i, j] = newBtn;

                    newBtn.FlatAppearance.MouseOverBackColor = btnBoard.FlatAppearance.MouseOverBackColor ;
                    newBtn.BackColor = isColBlack ? Color.Gray : Color.White;
                    newBtn.FlatAppearance.BorderColor = btnBoard.FlatAppearance.BorderColor;
                    newBtn.FlatAppearance.BorderSize = btnBoard.FlatAppearance.BorderSize;
                    newBtn.FlatStyle = btnBoard.FlatStyle;

                    newBtn.Size = new System.Drawing.Size(btnBoard.Width, btnBoard.Height);
                    newBtn.Tag = new Square(i, j);
                    pnt = new Point(currentWidth, currentHeight );
                    newBtn.Location = pnt;
                    newBtn.BackgroundImageLayout = ImageLayout.Stretch;

                    newBtn.BackgroundImage  = getImageBySign(board[z]);
                    
                    newBtn.Click += lastlbl_Click;

                    Controls.Add(newBtn);


                    currentWidth += btnBoard.Width;
                    isColBlack = !isColBlack;
                    z++;

                }

                isRowBlack = !isRowBlack;
                currentHeight += btnBoard.Height;

            }

            Controls.Remove(btnBoard);
            this.ResumeLayout(false);
        }

        void lastlbl_Click(object sender, EventArgs e)
        {
            Button b = (Button)sender;
            if (srcSquare != null)
            {
                // unselected
                if (matBoard[srcSquare.Row, srcSquare.Col] == b)
                {
                 
                    matBoard[srcSquare.Row, srcSquare.Col].FlatAppearance.BorderColor = Color.Blue;
                    srcSquare = null;
                }
                else
                {
                    dstSquare = (Square)b.Tag;
                    matBoard[dstSquare.Row, dstSquare.Col].FlatAppearance.BorderColor = Color.DarkGreen;

                    Thread t = new Thread(playMove);
                    t.Start();
                 //   t.IsBackground = true;
                    //playMove();
                }
            }
            else
            {
                srcSquare = (Square)b.Tag;
                matBoard[srcSquare.Row, srcSquare.Col].FlatAppearance.BorderColor = Color.DarkGreen;
            }
         
        }

        // messages should be according the protocol.
        // index is the message number in the protocol
        string[] messages =  {
            "Valid move",
            "Valid move - you made chess",
            "Invalid move - not your player",
            "Invalid move - destination is not free",
            "Invalid move - chess wil occure",
            "Invalid move - out of bounds",
            "Invalid move - illegeal movement with piece",
            "Invalid move - source and dest are equal",
            "Game over - check mate",
            "Unknown message"
            };


        string convertEngineToText(string m)
        {
            int res;
            bool b = int.TryParse(m, out res);

            if (!b || res < 0 || res >= messages.Length)
                return messages[messages.Length - 1];

            return messages[res];
        }
        


        void playMove()
        {
            if (isGameOver)
                return;


            try
            {
                 Invoke((MethodInvoker)delegate {

                     lblEngineCalc.Visible = true;
            
                     lblMove.Text = string.Format("Move from {0} to {1}", srcSquare, dstSquare);
                    lblMove.Visible = true;
                    //lblEngineCalc.Invalidate();
            
                    label2.Visible = false;
                    lblResult.Visible = false;

                    this.Refresh();
            

                    // should send pipe to engine
                    enginePipe.sendEngineMove(srcSquare.ToString() + dstSquare.ToString());
                    

                     // should get pipe from engine
                    string m = enginePipe.getEngineMessage();

                    if (!enginePipe.isConnected())
                    {
                        MessageBox.Show("Connection to engine has lost. Bye bye.");
                        this.Close();
                        return;
                    }

                    string res = convertEngineToText(m);

                    if (res.ToLower().StartsWith("game over"))
                    {
                        isGameOver = true;
                    }
                    else if (res.ToLower().StartsWith("valid"))
                    {
                        isCurPlWhite = !isCurPlWhite;

                        matBoard[dstSquare.Row, dstSquare.Col].BackgroundImage = matBoard[srcSquare.Row, srcSquare.Col].BackgroundImage;
                        matBoard[srcSquare.Row, srcSquare.Col].BackgroundImage = null;

                        matBoard[srcSquare.Row, srcSquare.Col].FlatAppearance.BorderColor = Color.Blue;
                        matBoard[dstSquare.Row, dstSquare.Col].FlatAppearance.BorderColor = Color.Blue;
                    
                    }

                    lblEngineCalc.Visible = false;
                    lblResult.Text = string.Format("{0}", res);
                    lblResult.Visible = true;
                    label2.Visible = true;
                    this.Refresh();
                 });
                
           
                }
                catch
                {

                }
                finally
                {
                    Invoke((MethodInvoker)delegate
                    {
                        if (srcSquare != null)
                            matBoard[srcSquare.Row, srcSquare.Col].FlatAppearance.BorderColor = Color.Blue;

                        if (dstSquare != null)
                            matBoard[dstSquare.Row, dstSquare.Col].FlatAppearance.BorderColor = Color.Blue;

                        dstSquare = null;
                        srcSquare = null;

                    });
                }

        }
        void changePipeHandler()
        {
            int row = 0, col = 0;
            char piece;
            string won;
            PieceSelection choice;
                while (true)
                {
                    try
                    {
                        string m = changePipe.getEngineMessage();
                    if (m.StartsWith("change"))
                    {
                        row = m[7] - '0';
                        col = m[9] - '0';
                        piece = m[11];
                        Invoke((MethodInvoker)delegate
                        {
                            matBoard[row, col].BackgroundImage = getImageBySign(piece);
                            this.Refresh();
                        });

                    }
                    else if (m.StartsWith("choice"))
                    {
                        if (m.Substring(7) == "white")
                        {
                            choice = new PieceSelection();
                            choice.initForm(true, enginePipe);
                        }
                        else
                        {
                            choice = new PieceSelection();
                            choice.initForm(false, enginePipe);
                        }
                        Invoke((MethodInvoker)delegate
                        {
                            choice.Show();
                        });
                    }
                    else if (m.StartsWith("mate"))
                    {
                        won = m.Substring(5);
                        switch (won)
                        {
                            case "white":
                                won = "Black";
                                break;
                            case "black":
                                won = "White";
                                break;
                        }
                        Invoke((MethodInvoker)delegate
                        {
                            MessageBox.Show(won + " Won.", "Check Mate!");
                            this.Close();
                            return;
                        });
                    }
                    else if (m.StartsWith("chat"))
                    {
                        Invoke((MethodInvoker)delegate
                        {
                            this.chatBox.AppendText("Opponent: " + m.Substring(10));
                            this.chatBox.AppendText(Environment.NewLine);
                        });
                    }
                    else if (m.StartsWith("wait"))
                    {
                        Invoke((MethodInvoker)delegate
                        {
                            this.wait.Visible = true;
                        });
                    }
                    else if (m.StartsWith("connect"))
                    {
                        Invoke((MethodInvoker)delegate
                        {
                            this.wait.Visible = false;
                        });
                    }
                    else if (m.StartsWith("turn"))
                    {
                        if (this.lblCurrentPlayer.Text == "White")
                        {
                            Invoke((MethodInvoker)delegate
                            {
                                this.lblCurrentPlayer.Text = "Black";
                            });
                        }
                        else if(this.lblCurrentPlayer.Text == "Black")
                        {
                            Invoke((MethodInvoker)delegate
                            {
                                this.lblCurrentPlayer.Text = "White";
                            });
                        }
                    }
                    else if (m.StartsWith("msg"))
                    {
                        Invoke((MethodInvoker)delegate
                        {
                            this.lblResult.Text = convertEngineToText(m[4].ToString());
                        });
                    }
                    else if (m.StartsWith("disconnect"))
                    {
                        Invoke((MethodInvoker)delegate{
                            MessageBox.Show("Opponent Disconnected!");
                            this.Close();
                        });
                    }
                    }
                    catch
                    {

                    }
                }
              }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
   

            enginePipe.sendEngineMove("quit");
            enginePipe.close();
            changePipe.close();
        }

        private void sendBtn_Click(object sender, EventArgs e)
        {
            string msg;
            this.chatBox.AppendText("You: " + this.msgBox.Text);
            this.chatBox.AppendText(Environment.NewLine);
            msg = this.msgBox.Text;
            this.msgBox.Text = "";
            lock (pipeLock)
            {
                this.chatPipe.sendEngineMove("chat " + msg);
            }
        }
        private void CheckEnterKeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)Keys.Return)

            {
                string msg;
                this.chatBox.AppendText("You: " + this.msgBox.Text);
                this.chatBox.AppendText(Environment.NewLine);
                msg = this.msgBox.Text;
                this.msgBox.Text = "";
                this.chatPipe.sendEngineMove("chat " + msg);
            }
        }

        private void label35_Click(object sender, EventArgs e)
        {

        }
    }
}

