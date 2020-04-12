namespace chessGraphics
{
    partial class PieceSelection
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.Queen = new System.Windows.Forms.Button();
            this.Rook = new System.Windows.Forms.Button();
            this.Bishop = new System.Windows.Forms.Button();
            this.Knight = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // Queen
            // 
            this.Queen.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.Queen.Location = new System.Drawing.Point(12, 32);
            this.Queen.Name = "Queen";
            this.Queen.Size = new System.Drawing.Size(129, 147);
            this.Queen.TabIndex = 0;
            this.Queen.UseVisualStyleBackColor = true;
            // 
            // Rook
            // 
            this.Rook.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.Rook.Location = new System.Drawing.Point(147, 32);
            this.Rook.Name = "Rook";
            this.Rook.Size = new System.Drawing.Size(129, 147);
            this.Rook.TabIndex = 1;
            this.Rook.UseVisualStyleBackColor = true;
            // 
            // Bishop
            // 
            this.Bishop.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.Bishop.Location = new System.Drawing.Point(282, 32);
            this.Bishop.Name = "Bishop";
            this.Bishop.Size = new System.Drawing.Size(129, 147);
            this.Bishop.TabIndex = 2;
            this.Bishop.UseVisualStyleBackColor = true;
            // 
            // Knight
            // 
            this.Knight.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.Knight.Location = new System.Drawing.Point(417, 32);
            this.Knight.Name = "Knight";
            this.Knight.Size = new System.Drawing.Size(129, 147);
            this.Knight.TabIndex = 3;
            this.Knight.UseVisualStyleBackColor = true;
            // 
            // PieceSelection
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(565, 215);
            this.Controls.Add(this.Knight);
            this.Controls.Add(this.Bishop);
            this.Controls.Add(this.Rook);
            this.Controls.Add(this.Queen);
            this.Name = "PieceSelection";
            this.Text = "Choose A Piece";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button Queen;
        private System.Windows.Forms.Button Rook;
        private System.Windows.Forms.Button Bishop;
        private System.Windows.Forms.Button Knight;
    }
}