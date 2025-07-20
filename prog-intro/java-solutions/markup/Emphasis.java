package markup;

import java.util.List;
public class Emphasis extends absclass {
    public Emphasis(List<MarkUp> marup) {
        super(marup);
    }
    public void toMarkdown(StringBuilder str) {
        super.toMarkdown(str,"*");
    }
    public void toHtml(StringBuilder str) {
        super.toHtml(str,"<em>","</em>");
    }
}