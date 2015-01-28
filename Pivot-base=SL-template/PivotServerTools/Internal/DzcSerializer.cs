using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Xml;
using System.Xml.Linq;

namespace PivotServerTools.Internal
{
	// Serialize a Collection's image data into Deep Zoom Collection XML.
	// Reference: http://www.getpivot.com/developer-info/image-content.aspx
	internal class DzcSerializer
	{
		XNamespace Xmlns { get { return "http://schemas.microsoft.com/deepzoom/2008"; } }
		int MaxLevel { get; set; }
		int TileDimension { get; set; }

		public static int DefaultMaxLevel = 8;
		public static int DefaultTileDimension = 256;
		public static bool IsAutoGenerateDzi = true;

		private Collection _collection;

		private DzcSerializer(Collection collection)
		{
			_collection = collection;

			MaxLevel = DefaultMaxLevel;
			TileDimension = DefaultTileDimension;
		}

		// Write a collection's image data as a DZC into an XmlWriter.
		public static void Serialize(XmlWriter xmlWriter, Collection collection)
		{
			DzcSerializer serializer = new DzcSerializer(collection);
			serializer.Write(xmlWriter);
		}

		// Write a collection's image data as a DZC into a TextWriter.
		public static void Serialize(TextWriter textWriter, Collection collection)
		{
			using (XmlWriter xmlWriter = XmlWriter.Create(textWriter))
			{
				Serialize(xmlWriter, collection);
			}
		}

		private void Write(XmlWriter outputWriter)
		{
			outputWriter.WriteStartDocument();
			XStreamingElement root = MakeDzcTree();
			root.WriteTo(outputWriter);
		}

		private XStreamingElement MakeDzcTree()
		{
			return new XStreamingElement(Xmlns + "Collection", MakeCollectionContent());
		}

		private IEnumerable<object> MakeCollectionContent()
		{
			yield return new XAttribute("MaxLevel", MaxLevel);
			yield return new XAttribute("TileSize", TileDimension);
			yield return new XAttribute("Format", "jpg");
			yield return new XAttribute("NextItemId", _collection.Items.Count);

			yield return MakeItems();
		}

		private XStreamingElement MakeItems()
		{
			int itemId = 0;
			var xmlItems = _collection.Items.Select(
				item => new XStreamingElement(Xmlns + "I", MakeItemContent(item, itemId++)));
			XStreamingElement items = new XStreamingElement(Xmlns + "Items", xmlItems);
			return items;
		}

		private IEnumerable<object> MakeItemContent(CollectionItem item, int id)
		{
			yield return new XAttribute("Id", id);
			yield return new XAttribute("N", id);
			yield return new XAttribute("IsPath", 1);

			bool hasProvider = (item.ImageProvider != null);
			if (!hasProvider)
			{
				yield return new XAttribute("Source", string.Empty);
			}
			else
			{
				Size size = item.ImageProvider.Size;

				string source = string.Empty;
				if (item.ImageProvider.DziPath != null)
					source = item.ImageProvider.DziPath;
				else
				if (IsAutoGenerateDzi && (size.Width > TileDimension || size.Height > TileDimension)) // Only provide an auto-DZI if the image is larger than a collection tile
					source = DziSerializer.MakeDziPath(_collection.CollectionKey, id);

				yield return new XAttribute("Source", source);
				yield return new XStreamingElement(Xmlns + "Size",
					new XAttribute("Width", size.Width), new XAttribute("Height", size.Height));
			}
		}
	}
}