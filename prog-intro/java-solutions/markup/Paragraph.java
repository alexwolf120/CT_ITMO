package markup;

import java.util.List;

public class Paragraph extends MarkUp {
    protected List<MarkUp> marup;
    public Paragraph(List<MarkUp> marup) {
        this.marup = marup;
    }
    public void toMarkdown(StringBuilder str) {
        for (MarkUp md : marup) {
            md.toMarkdown(str);
        }
    }
    public void toHtml(StringBuilder str) {
        for (MarkUp hl : marup) {
            hl.toHtml(str);
        }
    }

}