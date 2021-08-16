using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VisualScriptingEnv
{
    public class BoundingBox
    {
        public BoundingBox(CanvasPoint point, CanvasSize size)
        {
            this.Location = point;
            this.Size = size;
        }
        public BoundingBox(double x, double y, double w, double h)
        {
            this.Location = new CanvasPoint(x, y);
            this.Size = new CanvasSize(w, h);
        }

        public CanvasPoint Location { get; set; }
        public CanvasSize Size { get; set; }
        public double Left
        {
            get
            {
                return this.Location.X;
            }
        }
        public double Right
        {
            get
            {
                return (this.Location.X + this.Size.Height);
            }
        }
        public double Top
        {
            get
            {
                return this.Location.Y;
            }
        }
        public double Bottom
        {
            get
            {
                return (this.Location.Y + this.Size.Width);
            }
        }

        public CanvasPoint this[int index]
        {
            get
            {
                if ((index < 0) || (index > 3)) throw new IndexOutOfRangeException("Index must be between [0-3]. Bounding Boxes have only 4 Points.");
                switch (index)
                {
                    case 0: return new CanvasPoint(this.Top, this.Left);
                    case 1: return new CanvasPoint(this.Top, this.Right);
                    case 2: return new CanvasPoint(this.Bottom, this.Right);
                    case 3: return new CanvasPoint(this.Bottom, this.Left);
                    default: throw new IndexOutOfRangeException("Index must be between [0-3]. Bounding Boxes have only 4 Points.");
                }
            }
        }
        public CanvasPoint[] GetPoints()
        {
            CanvasPoint[] cpOut = {
                new CanvasPoint(this.Top, this.Left),
                new CanvasPoint(this.Top, this.Right),
                new CanvasPoint(this.Bottom, this.Right),
                new CanvasPoint(this.Bottom, this.Left)
            };
            return cpOut;
        }
        public bool Contains(CanvasPoint point, bool strict = true)
        {
            bool boolOut = true;
            if (strict)
            {
                boolOut = boolOut && (point.X > this.Left);
                boolOut = boolOut && (point.X < this.Right);
                boolOut = boolOut && (point.Y > this.Top);
                boolOut = boolOut && (point.Y < this.Bottom);
            }
            else
            {
                boolOut = boolOut && (point.X >= this.Left);
                boolOut = boolOut && (point.X <= this.Right);
                boolOut = boolOut && (point.Y >= this.Top);
                boolOut = boolOut && (point.Y <= this.Bottom);
            }
            return boolOut;
        }
        public bool Contains(BoundingBox box, bool strict = true)
        {
            bool boolOut = true;
            if (strict)
            {
                boolOut = boolOut && (box.Left > this.Left);
                boolOut = boolOut && (box.Right < this.Right);
                boolOut = boolOut && (box.Top > this.Top);
                boolOut = boolOut && (box.Bottom < this.Bottom);
            }
            else
            {
                boolOut = boolOut && (box.Left >= this.Left);
                boolOut = boolOut && (box.Right <= this.Right);
                boolOut = boolOut && (box.Top >= this.Top);
                boolOut = boolOut && (box.Bottom <= this.Bottom);
            }
            return boolOut;
        }
        public static BoundingBox FromLTRB(double l, double t, double r, double b)
        {
            double x, y, w, h;
            if (r <= l)
            {
                x = l;
                w = (r - l);
            }
            else
            {
                x = r;
                w = (l - r);
            }
            if (b <= t)
            {
                y = t;
                h = (b - t);
            }
            else
            {
                y = b;
                h = (t - b);
            }
            BoundingBox bbOut = new BoundingBox(new CanvasPoint(x, y), new CanvasSize(w, h));
            return bbOut;
        }
        public BoundingBox Inflate(CanvasSize s)
        {
            return new BoundingBox(this.Location, new CanvasSize((this.Size.Width + s.Width), (this.Size.Height + s.Height)));
        }
        public BoundingBox Offset(CanvasSize s)
        {
            return new BoundingBox(new CanvasPoint((this.Location.X + s.Width), (this.Location.Y + s.Height)), new CanvasSize((this.Size.Width + s.Width), (this.Size.Height + s.Height)));
        }
        public BoundingBox Offset(double x1, double x2, double y1, double y2)
        {
            return new BoundingBox(new CanvasPoint((this.Location.X + x1), (this.Location.Y + y1)), new CanvasSize((this.Size.Width + x2), (this.Size.Height + y2)));
        }

        //public static BoundingBox operator + (BoundingBox A, BoundingBox B)
        //{
        //    return BoundingBox.Union(A, B);
        //}
        //public static BoundingBox operator -(BoundingBox A, BoundingBox B)
        //{
        //    return BoundingBox.Intersect(A, B);
        //}
        //public static BoundingBox Intersect(BoundingBox a, BoundingBox b)
        //{
        //    if (CheckIntersection(a, b))
        //    {
        //        if (a.Contains(b, false)) return b;
        //        else if (b.Contains(a, false)) return a;
        //        else
        //        {
        //            int aVSb = 0;
        //            foreach (CanvasPoint p in a.GetPoints())
        //            {
        //                if (b.Contains(p, false))
        //                {
        //                    aVSb++;
        //                }
        //            }
        //            foreach (CanvasPoint p in b.GetPoints())
        //            {
        //                if (a.Contains(p, false))
        //                {
        //                    aVSb--;
        //                }
        //            }

        //            double x, y, w, h;
        //            if (aVSb == 0)
        //            {
        //                if (a.Location.X <= b.Location.X) x = a.Location.X;
        //                else x = b.Location.X;
        //                if (a.Location.X <= b.Location.X) x = a.Location.X;
        //                else x = b.Location.X;
        //            }
        //            BoundingBox bbOut = new BoundingBox(x, y, w, h);
        //            return bbOut;
        //        }
        //    }
        //    else return null;
        //}
        public static bool CheckIntersection(BoundingBox a, BoundingBox b)
        {
            foreach (CanvasPoint p in a.GetPoints()) if (b.Contains(p, false)) return true;
            foreach (CanvasPoint p in b.GetPoints()) if (a.Contains(p, false)) return true;
            return false;
        }
        //public static BoundingBox Union(BoundingBox a, BoundingBox b)
        //{

        //    BoundingBox bbOut = new BoundingBox(x, y, w, h);
        //}
    }

    public class CanvasPoint : IEquatable<CanvasSize>, IEquatable<CanvasPoint>
    {
        public CanvasPoint(double x, double y)
        {
            this.X = x;
            this.Y = y;
        }

        public double X { get; set; }
        public double Y { get; set; }

        public override string ToString()
        {
            return ($"CanvasPoint({this.X.ToString()}, {this.Y.ToString()})");
        }

        public static CanvasSize operator +(CanvasPoint A, CanvasPoint B)
        {
            return new CanvasSize((A.X + B.X), (A.Y + B.Y));
        }
        public static CanvasSize operator -(CanvasPoint A, CanvasPoint B)
        {
            return new CanvasSize((A.X - B.X), (A.Y - B.Y));
        }
        public static bool operator ==(CanvasPoint A, CanvasPoint B)
        {
            return ((A.X == B.X) && (A.Y == B.Y));
        }
        public static bool operator !=(CanvasPoint A, CanvasPoint B)
        {
            return ((A.X != B.X) && (A.Y != B.Y));
        }
        public override bool Equals(object obj)
        {
            if (!(obj is CanvasPoint)) return false;
            CanvasPoint c = obj as CanvasPoint;
            return ((this.X == c.X) && (this.Y == c.Y));
        }
        public override int GetHashCode()
        {
            return base.GetHashCode();
        }
        public bool Equals(CanvasPoint other)
        {
            CanvasPoint s = this;
            return ((this.X == other.X) && (this.Y == other.Y));
        }
        public bool Equals(CanvasSize other)
        {
            return ((this.X == other.Width) && (this.Y == other.Height));
        }
        public static implicit operator CanvasPoint(CanvasSize v)
        {
            CanvasPoint s = new CanvasPoint(v.Width, v.Height);
            return s;
        }
    }
    public class CanvasSize : IEquatable<CanvasSize>, IEquatable<CanvasPoint>
    {
        public CanvasSize(double w, double h)
        {
            this.Width = w;
            this.Height = h;
        }

        public double Width { get; set; }
        public double Height { get; set; }

        public override string ToString()
        {
            return ($"CanvasSize({this.Width.ToString()}, {this.Height.ToString()})");
        }

        public static CanvasSize operator +(CanvasSize A, CanvasSize B)
        {
            return new CanvasSize((A.Width + B.Width), (A.Height + B.Height));
        }
        public static CanvasSize operator -(CanvasSize A, CanvasSize B)
        {
            return new CanvasSize((A.Width - B.Width), (A.Height - B.Height));
        }
        public static bool operator ==(CanvasSize A, CanvasSize B)
        {
            return ((A.Width == B.Width) && (A.Height == B.Height));
        }
        public static bool operator !=(CanvasSize A, CanvasSize B)
        {
            return ((A.Width != B.Width) && (A.Height != B.Height));
        }
        public override bool Equals(object obj)
        {
            if (!(obj is CanvasSize)) return false;
            CanvasSize c = obj as CanvasSize;
            return ((this.Width == c.Width) && (this.Height == c.Height));
        }
        public override int GetHashCode()
        {
            return base.GetHashCode();
        }
        public bool Equals(CanvasPoint other)
        {
            CanvasPoint s = this;
            return ((this.Width == other.X) && (this.Height == other.Y));
        }
        public bool Equals(CanvasSize other)
        {
            return ((this.Width == other.Width) && (this.Height == other.Height));
        }
        public static implicit operator CanvasSize(CanvasPoint v)
        {
            CanvasSize s = new CanvasSize(v.X, v.Y);
            return s;
        }
    }
}
