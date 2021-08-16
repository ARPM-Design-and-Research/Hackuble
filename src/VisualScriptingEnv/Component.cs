using System;
using System.Collections;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using GUICLR;

namespace VisualScripting
{
    public class Component : IDisposable
    {
        GUICLR.Component _component;
        GUICLR.BoundingBox _boundingBox;
        Guid _instanceGuid;
        private Color _color;

        public Component()
        {
            _component = new GUICLR.Component("Component");
            _boundingBox = _component.getBoundingBox();
            this._instanceGuid = Guid.NewGuid();
        }
        public Component(string title)
        {
            _component = new GUICLR.Component(title);
            _color = Color.FromArgb(255, 103, 0);
            int r, g, b;
            r = ((int)_color.R);
            g = ((int)_color.G);
            b = ((int)_color.B);
            _component.setColor(r, g, b);
            this._instanceGuid = Guid.NewGuid();
        }
        public Component(string title, Color color)
        {
            _component = new GUICLR.Component(title);
            _color = color;
            int r, g, b;
            r = ((int)_color.R);
            g = ((int)_color.G);
            b = ((int)_color.B);
            _component.setColor(r, g, b);
            this._instanceGuid = Guid.NewGuid();
        }

        //BoundingBox

        public Guid InstanceGuid { get => _instanceGuid; }
        public ComponentCollection Children { get; }
        public string Name
        {
            get
            {
                return _component.getTitle();
            }
            set
            {
                _component.setTitle(value);
            }
        }
        public Color Color
        {
            get
            {
                return _color;
            }
            set
            {
                _color = value;
                int r, g, b;
                r = ((int)_color.R);
                g = ((int)_color.G);
                b = ((int)_color.B);
                _component.setColor(r, g, b);
            }
        }

        public void AddSlider(SliderState sliderState, string title, float currentValue, float startValue, float endValue)
        {
            _component.addSlider(title, sliderState, currentValue, startValue, endValue);
        }

        public static bool operator == (Component A, Component B)
        {
            return (A.InstanceGuid == B.InstanceGuid);
        }
        public static bool operator != (Component A, Component B)
        {
            return (A.InstanceGuid != B.InstanceGuid);
        }
        public override bool Equals(object obj)
        {
            if (!(obj is Component)) return false;
            Component c = obj as Component;
            return (this.InstanceGuid == c.InstanceGuid);
        }
        public bool Equals(Component other)
        {
            return (this.InstanceGuid == other.InstanceGuid);
        }
        public override int GetHashCode()
        {
            return base.GetHashCode();
        }
        public override string ToString()
        {
            return this.Name;
        }
        public void Dispose()
        {
            _component.Dispose();
        }
        ~Component()
        {
            this.Dispose();
        }
    }

    public class ComponentCollection : IList<Component>, ICollection<Component>, IEnumerable<Component>
    {
        private Component[] _components;
        private Component _parent;

        //BoundingBox

        public Component this[int index]
        {
            get
            {
                return this._components[index];
            }
            set
            {
                if (index < 0) throw new IndexOutOfRangeException("Index cannot be <0.");
                if (this._components.Length > index)
                {
                    this._components[index] = value;
                }
                else throw new IndexOutOfRangeException($"Index {index} is out of range [0 - {(this._components.Length - 1)}].");
            }
        }

        public int Count => _components.Length;

        public bool IsReadOnly => false;

        public void Add(Component item)
        {
            this._components.Append<Component>(item);
        }

        public void Clear()
        {
            _components = new Component[] { };
        }

        public bool Contains(Component item)
        {
            for (int i = 0; i < this._components.Length; i++)
            {
                if (item == this._components[i])
                {
                    return true;
                }
            }
            return false;
        }

        public void CopyTo(Component[] array, int arrayIndex)
        {
            this._components.CopyTo(array, arrayIndex);
        }

        public IEnumerator<Component> GetEnumerator()
        {
            return (IEnumerator<Component>)this._components.GetEnumerator();
        }

        public int IndexOf(Component item)
        {
            for (int i = 0; i < this._components.Length; i++)
            {
                if (item == this._components[i])
                {
                    return i;
                }
            }
            return -1;
        }

        public void Insert(int index, Component item)
        {
            int n = this._components.Length + 1;
            Component[] newarr = new Component[n];
            for (int i = 0; i < n + 1; i++)
            {
                if (i < index - 1)
                    newarr[i] = this._components[i];
                else if (i == index - 1)
                    newarr[i] = item;
                else
                    newarr[i] = this._components[i - 1];
            }
            this._components = newarr;
        }

        public bool Remove(Component item)
        {
            int i = this.IndexOf(item);
            if (i != -1)
            {
                this.RemoveAt(i);
                return true;
            }
            else return false;
        }

        public void RemoveAt(int index)
        {
            int n = this._components.Length + 1;
            Component[] newarr = new Component[n];
            for (int i = 0; i < n - 1; i++)
            {
                if (i < index - 1)
                    newarr[i] = this._components[i];
                else if (i == index - 1)
                    continue;
                else
                    newarr[i] = this._components[i + 1];
            }
            this._components = newarr;
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return _components.GetEnumerator();
        }
    }

    
}
