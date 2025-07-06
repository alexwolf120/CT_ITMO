package markup;

import java.util.List;

public class Strikeout extends absclass {
    public Strikeout(List<MarkUp> marup) {
        super(marup);
    }
    public void toMarkdown(StringBuilder str) {
        super.toMarkdown(str, "~");
    }
    public void toHtml(StringBuilder str) {
        super.toHtml(str,"<s>","</s>");
    }
}